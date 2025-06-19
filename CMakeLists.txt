cmake_minimum_required(VERSION 3.10)
project(HeavenConnection)

# 设置 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译器为 clang++
set(CMAKE_CXX_COMPILER "clang++")

# 设置构建类型为 Release（启用优化），可改为 Debug 或 RelWithDebInfo
set(CMAKE_BUILD_TYPE Release)

# 可选：将 -O3 改为 -O2（某些场景下更稳定）
string(REPLACE "-O3" "-O2" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 查找 MySQL 依赖
find_package(PkgConfig REQUIRED)
pkg_check_modules(MYSQL REQUIRED mysqlclient)

# 源文件列表
set(SOURCES
    main.cpp
    connection.cpp
    connection_pool.cpp
)

add_executable(HeavenConnection ${SOURCES})

# 包含 MySQL 头文件目录
target_include_directories(HeavenConnection PRIVATE ${MYSQL_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})

# 链接 MySQL 和 pthread（必须）
target_link_libraries(HeavenConnection PRIVATE ${MYSQL_LIBRARIES} pthread)