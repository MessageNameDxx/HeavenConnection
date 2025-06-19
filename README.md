HeavenConnection - C++ MySQL 连接池实现
一个基于 C++11 的轻量级数据库连接池实现，适用于高并发场景下的 MySQL 数据库连接管理。

🧠 项目简介
该项目实现了一个线程安全的 MySQL 数据库连接池，支持连接复用、超时控制、空闲连接回收等核心功能。通过多线程测试验证了其在并发访问下的稳定性和性能优势。

✅ 功能特性
✅ 线程安全设计（使用 std::mutex, std::condition_variable）
✅ 支持连接复用，提升系统吞吐量
✅ 自动回收空闲连接（后台扫描线程）
✅ 连接获取超时机制，防止死锁
✅ 使用 shared_ptr 管理连接生命周期，自动归还连接
✅ 多线程并发测试验证稳定性
🧰 技术栈
类别	技术
编程语言	C++17
编译器	clang++ / g++
构建工具	CMake
数据库	MySQL C API (libmysqlclient)
操作系统	Linux（Ubuntu 推荐）
📁 项目结构
.
├── connection.h/cpp        # 数据库连接封装类
├── connection_pool.h/cpp   # 连接池核心实现
├── main.cpp                # 测试程序（多线程并发插入）
├── public.h                # 日志宏定义
├── mysql.cnf               # 配置文件（IP、端口、用户等）
└── CMakeLists.txt          # 构建配置
🛠️ 编译与运行
1. 安装依赖
bash
sudo apt update
sudo apt install libmysqlclient-dev cmake clang
2. 构建项目
bash
mkdir build && cd build
cmake ..
make
3. 运行测试
bash
./HeavenConnection
⚠️ 注意：

确保 mysql.cnf 在正确路径；
确保 MySQL 服务已启动，并创建了 test_db 数据库和 test_user 表；
推荐使用 localhost 模式以启用 Unix Socket 提升性能。
🔍 示例输出
bash
time: 0.48s
表示 10000 次并发插入操作耗时约 100ms（取决于硬件环境）。

⚙️ 配置说明
编辑 mysql.cnf 可自定义连接参数：

ini
ip=localhost
port=
username=
password=
database=test_db
initSize=10
maxSize=1024
maxIdleTime=60
connectionTimeout=100
📌 注意事项
使用 shared_ptr + lambda 删除器确保连接归还；
后台线程目前为 detach() 模式，主线程退出后仍可能短暂运行；
建议添加 running_ 标志位来优雅关闭后台线程；
SQL 拼接建议后续改为参数化查询以增强安全性。
🚀 待优化方向（欢迎贡献）
功能	描述
SQL 参数化支持	防止注入攻击
更完善的日志系统	支持日志级别、输出到文件
连接健康检查	主动 ping 检查连接有效性
自动扩容机制	动态增加连接数
单元测试	使用 Google Test 编写测试用例
异常处理	捕获并处理潜在异常
💬 联系作者
如果你有任何建议或问题，请在 GitHub Issues 中提交反馈！
