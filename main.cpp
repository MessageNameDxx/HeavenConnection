#include <iostream>
#include <thread>
#include "connection.h"
#include "connection_pool.h"
using namespace std;


int main(){
    clock_t start = clock();
    thread t1([](){
        ConnectionPool *conn = ConnectionPool::getConnectionPool();
        for(int i = 0; i < 250; i++){
            char sql[1024] = {0};
            sprintf(sql, "insert into test_user(name, age, sex) values('%s', '%s', '%s')", "Tom", "20", "M");
            shared_ptr<Connection> conn_ptr = conn->getConnection();
            conn_ptr->update(sql);
        }
    });
    thread t2([](){
        ConnectionPool *conn = ConnectionPool::getConnectionPool();
        
        for(int i = 0; i < 250; i++){
            char sql[1024] = {0};
            sprintf(sql, "insert into test_user(name, age, sex) values('%s', '%s', '%s')", "Tom", "20", "M");
            shared_ptr<Connection> conn_ptr = conn->getConnection();
            conn_ptr->update(sql);
        }
    });
    thread t3([](){
        ConnectionPool *conn = ConnectionPool::getConnectionPool();
        for(int i = 0; i < 250; i++){
            char sql[1024] = {0};
            sprintf(sql, "insert into test_user(name, age, sex) values('%s', '%s', '%s')", "Tom", "20", "M");
            shared_ptr<Connection> conn_ptr = conn->getConnection();
            conn_ptr->update(sql);
        }
    });
    thread t4([](){
        ConnectionPool *conn = ConnectionPool::getConnectionPool();
        for(int i = 0; i < 250; i++){
            char sql[1024] = {0};
            sprintf(sql, "insert into test_user(name, age, sex) values('%s', '%s', '%s')", "Tom", "20", "M");
            shared_ptr<Connection> conn_ptr = conn->getConnection();
            conn_ptr->update(sql);
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    clock_t end = clock();
    cout << "time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
    return 0;
}

#if 0
for(int i = 0; i < 10000; i++){
        // Connection conn;
        // char sql[1024] = {0};
        // sprintf(sql, "insert into test_user(name, age, sex) values('%s', '%s', '%s')", "Tom", "20", "M");
        // conn.connect("localhost", 3306, "newuser", "MySql@dxx20050425", "test_db");
        // conn.update(sql);
        shared_ptr<Connection> conn_ptr = conn->getConnection();
        char sql[1024] = {0};
        sprintf(sql, "insert into test_user(name, age, sex) values('%s', '%s', '%s')", "Tom", "20", "M");
        conn_ptr->update(sql);
    }
#endif