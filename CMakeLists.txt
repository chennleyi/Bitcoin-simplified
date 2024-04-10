cmake_minimum_required(VERSION 3.0)   #设置cmake最低版本
project(bitcoin)                      #设置项目名称
set(CMAKE_CXX_STANDARD 17)            #设置C++版本
set(CMAKE_CXX_FLAGS "-lgmp")          #添加C++编译flag

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_compile_options(-w)

include_directories(libs)             #将给定的目录添加到编译器用于搜索包含文件的目录中。相对路径被解释为相对于当前源目录。
include_directories(include)

include(FetchContent)
FetchContent_Declare(
    leveldb
    GIT_REPOSITORY https://github.com/google/leveldb.git
    GIT_TAG 1.22
)
FetchContent_MakeAvailable(leveldb) #获取leveldb库

FetchContent_Declare(
    cereal
    GIT_REPOSITORY https://github.com/USCiLab/cereal.git
    GIT_TAG v1.3.1
)
FetchContent_MakeAvailable(cereal) #获取cereal库

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG v3.1.0
)
FetchContent_MakeAvailable(cxxopts) #获取cxxopts库


if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()

add_subdirectory(libs/ecdsa)

FetchContent_Declare(
     secp256k1
     GIT_REPOSITORY https://github.com/bitcoin-core/secp256k1.git
     GIT_TAG v0.4.1
)
FetchContent_MakeAvailable(secp256k1) #获取sep256k1库

FetchContent_Declare(
     spdlog
     GIT_REPOSITORY https://github.com/gabime/spdlog.git
     GIT_TAG v1.x
)
FetchContent_MakeAvailable(spdlog) #spdlog

add_library(sha2 STATIC libs/sha256/SHA256.h libs/sha256/SHA256.cpp) #生成静态库

include_directories(include)
include_directories(src)

enable_testing()

add_executable(
  hello_test
  ./test/test.cpp
  ./src/block.h
  ./src/block.cpp 
  ./src/proofofwork.h
  ./src/proofofwork.cpp
  ./src/transaction.h
  ./src/transaction.cpp
  ./src/blockchain.h
  ./src/blockchain.cpp
  ./src/utils.h
  ./src/utils.cpp
  ./src/wallet.h
  ./src/wallet.cpp
)
target_link_libraries(
  hello_test GTest::gtest_main sha2 ecdsa++ leveldb cereal cxxopts spdlog ${CMAKE_CXX_FLAGS} ${OPENSSL_LIBRARIES}
)

include(GoogleTest)
gtest_discover_tests(hello_test)