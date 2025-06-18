cmake_minimum_required(VERSION 3.10)
project(HeavenConnection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MYSQL REQUIRED mysqlclient)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
    main.cpp
    connection.cpp
    connection_pool.cpp
)

add_executable(HeavenConnection ${SOURCES})

target_include_directories(HeavenConnection PRIVATE ${MYSQL_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
target_link_libraries(HeavenConnection PRIVATE ${MYSQL_LIBRARIES})
