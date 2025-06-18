#pragma once
#include "public.h"
#include "connection.h"
#include <string>
#include <mysql/mysql.h>
#include <mutex>
#include <atomic>
#include<queue>

class ConnectionPool
{
public:
    // 获取连接池对象
    static ConnectionPool* getConnectionPool();
    MYSQL *getConnection();
    void releaseConnection(MYSQL *conn);
    void destory();

private:
    // 单例模式,私有化构造函数，
    ConnectionPool();
    // 
    ~ConnectionPool();
    //从配置文件中加载配置项
    bool loadConfigFile(); 
    void init(string url, string user, string password, string dbname, int port, int maxConn);
    Connection *getFreeConnection();

private:
    static ConnectionPool *pool;

    string ip_;            // ip地址
    unsigned short port_;  // 端口号
    string username_;      // 用户名
    string password_;      // 密码
    string dbname_;        // 数据库名称
    int initSize_;         // 初始化连接数
    int maxSize_;          // 最大连接数
    int maxIdleTime_;      // 最大空闲时间
    int connectionTimeout_;// 连接超时时间

    queue<Connection*> connectionQueue_;// 连接队列
    mutex queueMutex_;     // 连接队列的线程安全互斥锁
    atomic_int connectionCount_; // 连接池中当前连接总数
};