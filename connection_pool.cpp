#include "connection_pool.h"
#include "connection.h"
#include "public.h"
#include <mutex>

ConnectionPool* ConnectionPool::getConnectionPool(){
    //线程安全的懒汉单单例子模式
    static ConnectionPool pool;
    return &pool;
}

bool ConnectionPool::loadConfigFile(){
    //创建一个配置文件对象
    string configPath = "/home/dxx/HeavenConnection/mysql.cnf"; // 绝对路径
    FILE *fp = fopen(configPath.c_str(), "r");
    if (fp == nullptr){
        LOG("mysql.cnf open error");
        return false;
    }

    //逐行读取
    while(!feof(fp)){
        char line[1024] = {0};
        fgets(line, 1024, fp);
        string str(line);
        int idx = str.find('=', 0);
        if (idx == -1){
            continue;
        }
        int endIdx = str.find('\n', idx);
        string key = str.substr(0, idx);
        string value = str.substr(idx + 1, endIdx - idx - 1);
        if(key == "ip"){
            ip_ = value;
        }
        else if(key == "port"){
            port_ = atoi(value.c_str());
        }
        else if(key == "username"){
            username_ = value;
        }
        else if(key == "password"){
            password_ = value;
        }
        else if(key == "database"){
            dbname_ = value;
        }
        else if(key == "initSize"){
            initSize_ = atoi(value.c_str());
        }
        else if(key == "maxSize"){
            maxSize_ = atoi(value.c_str());
        }
        else if(key == "maxIdleTime"){
            maxIdleTime_ = atoi(value.c_str());
        }
        else if(key == "connectionTimeout"){
            connectionTimeout_ = atoi(value.c_str());
        }
    }
    return true;
}

ConnectionPool::ConnectionPool(){ 
    // 加载配置文件
    if(!loadConfigFile()){
        LOG("loadConfigFile fail");
        return;
    }

    // 初始化数据库连接池
    for(int i = 0; i < initSize_; ++i){
        Connection* conn = new Connection;
        conn->connect(ip_, port_, username_, password_, dbname_);
        conn->refreshAliveTime(); // 刷新连接存活时间
        connectionQueue_.push(conn);
        connectionCount_++;
    }

    // 启动新线程，作为连接生产者1
    thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
    produce.detach();

    // 启动新线程，扫描连接池中的连接，如果连接存活时间超过最大空闲时间，则关闭连接
    thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
    scanner.detach();
}

void ConnectionPool::produceConnectionTask(){
    // 每隔一段时间，检查连接池中的连接数量，如果数量小于最大连接数，则创建新的连接
    while(true){
        unique_lock<mutex> lock(queueMutex_);
        while(!connectionQueue_.empty()){
            condition_.wait(lock); // 等待条件变量
        }

        // 数量没有到上限，则创建新连接
        if(connectionCount_ < maxSize_){
            Connection *conn = new Connection();
            conn->connect(ip_, port_, username_, password_, dbname_);
            conn->refreshAliveTime();
            connectionQueue_.push(conn);
            connectionCount_++;
        }
    }

    // 通知消费者线程，连接池有新连接了，可以消费连接了
    condition_.notify_all();
}

shared_ptr<Connection> ConnectionPool::getConnection(){
    // 获取连接，如果连接池为空，则等待
    unique_lock<mutex> lock(queueMutex_);
    while(connectionQueue_.empty()){
        condition_.wait_for(lock, chrono::milliseconds(connectionTimeout_));
        if(connectionQueue_.empty()){
            LOG("the connection acquisition timeout occurs");
            return nullptr;
        }
    }
    // 获取连接
    shared_ptr<Connection> conn(connectionQueue_.front(), [&](Connection *conn_){
        //连接使用完毕，归还连接
        unique_lock<mutex> lock(queueMutex_);
        conn_->refreshAliveTime();
        connectionQueue_.push(conn_);
    });
    connectionQueue_.pop();
    //消费完毕，通知生产者
    condition_.notify_all();
    return conn;
}

void ConnectionPool::scannerConnectionTask(){
    //扫描超时的连接
    while(true){
        //休眠一段时间
        this_thread::sleep_for(chrono::seconds(maxIdleTime_));
        //获取锁
        unique_lock<mutex> lock(queueMutex_);
        //扫描连接
        while(connectionCount_ > initSize_){
            Connection *conn = connectionQueue_.front();
            if(conn->getAliveTime() >= (maxIdleTime_ * 1000)){
                connectionQueue_.pop();
                connectionCount_--;
                LOG("sanner a timeout connection");
                delete conn;
            }
            else{
                //队头连接未超时，剩下的显然不用考虑，直接跳出循环
                break;
            }
        }
    }
}