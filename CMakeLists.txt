cmake_minimum_required(VERSION 3.10)
project(CacheTests)

set(CMAKE_CXX_STANDARD 17)

add_executable(run_tests tests.cpp node.cpp LRU.cpp LFU.cpp fibonacciwrapper.cpp icache.cpp
        LFU.cpp
        LFU.h
        LRU.cpp
        LRU.h
        fibwrapper.cpp
        fibwrapper.h
        icache.cpp
        icache.h
        node.cpp
        node.h
        tests.cpp)

add_subdirectory(googletest)

target_link_libraries(run_tests gtest gtest_main)