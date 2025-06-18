#include <iostream>
#include "connection.h"
using namespace std;


int main(){
    Connection conn;
    char sql[1024] = {0};
    sprintf(sql, "insert into test_user(name, age, sex) values('%s', '%s', '%s')", "Tom", "20", "M");
    conn.connect("127.0.0.1", 3306, "newuser", "MySql@dxx20050425", "test_db");
    conn.update(sql);
}