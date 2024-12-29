cmake_minimum_required(VERSION 3.10)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab02
        main.cpp
        src/gauss.cpp
        include/gauss.h)

target_include_directories(lab02 PRIVATE include)