#include "connection_pool.h"
#include "public.h"

ConnectionPool* ConnectionPool::getConnectionPool(){
    //线程安全的懒汉单单例子模式
    static ConnectionPool pool;
    return &pool;
}

bool ConnectionPool::loadConfigFile(){
    //创建一个配置文件对象
    FILE *fp = fopen("mysql.cnf", "r");
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
        else if(key == "dbname"){
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
        connectionQueue_.push(conn);
        connectionCount_++;
    }

    // 启动新线程，作为连接生产者1
    thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));

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
    shared_ptr<Connection> conn(connectionQueue_.front());
    connectionQueue_.pop();
    //消费完毕，通知生产者
    condition_.notify_all();
    return conn;
}