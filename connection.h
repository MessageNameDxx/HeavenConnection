#pragma once

#include <mysql/mysql.h>
#include <string>
#include <iostream>
#include <ctime>

using namespace std;

class Connection{
    public:
    //构造函数
    Connection();
    //析构函数
    ~Connection();
    //连接数据库
    bool connect(string ip, unsigned short port, string user, string password, string dbname);
    //更新数据
    bool update(string sql);
    //查询数据
    MYSQL_RES *query(string sql);
    //刷新连接起始时间
    void refreshAliveTime() { aliveTime_ = clock(); }
    //获取连接存活时间
    clock_t getAliveTime() const { return clock() - aliveTime_; }
private:
    MYSQL *conn_; //数据库连接句柄
    clock_t aliveTime_; //记录进入空闲状态后的存活时间
};