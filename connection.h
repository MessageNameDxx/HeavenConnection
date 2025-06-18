#pragma once

#include <mysql/mysql.h>
#include <string>
#include <iostream>

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
private:
    MYSQL *conn_; //数据库连接句柄
};