#include "public.h"
#include "connection.h"

using namespace std;

Connection::Connection(){
    // 初始化
    conn_ = mysql_init(nullptr);
}

Connection::~Connection(){
    // 关闭连接
    if(conn_ != nullptr){
        mysql_close(conn_);
    }
}

bool Connection::connect(string ip, unsigned short port, string user, string password, string dbname){
    // 连接数据库
    MYSQL *conn = mysql_real_connect(conn_, ip.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);
    return conn != nullptr;
}

bool Connection::update(string sql){
    // 执行sql语句 -- 更新语句
    if(mysql_query(conn_, sql.c_str())){
        LOG("update error:" + sql)
        return false;
    }
    return true;
}

MYSQL_RES* Connection::query(string sql){
    // 执行sql语句 -- 查询语句
    if(mysql_query(conn_, sql.c_str())){
        LOG("query error:" + sql)
        return nullptr;
    }
    // 获取结果集
    return mysql_store_result(conn_);
}