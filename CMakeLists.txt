cmake_minimum_required(VERSION 3.0)   #设置cmake最低版本
project(bitcoin)                      #设置项目名称
set(CMAKE_CXX_STANDARD 17)            #设置C++版本
include_directories(libs)             #将给定的目录添加到编译器用于搜索包含文件的目录中。相对路径被解释为相对于当前源目录。

include(FetchContent)
FetchContent_Declare(
    leveldb
    GIT_REPOSITORY https://github.com/google/leveldb.git
    GIT_TAG 1.22
)
FetchContent_MakeAvailable(leveldb) #获取leveldb库

add_library(sha2 STATIC libs/sha256/SHA256.h libs/sha256/SHA256.cpp) #生成静态库
add_executable(main ./src/main.cpp  # 编译成目标文件
                 ./src/block.h
                 ./src/block.cpp 
                 ./src/proofofwork.h
                 ./src/proofofwork.cpp
                 ./src/blockchain.h
                 ./src/blockchain.cpp
                 ./src/utils.h
                 ./src/utils.cpp
)

target_link_libraries(main PRIVATE sha2 leveldb) #链接静态库