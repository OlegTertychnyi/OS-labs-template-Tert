cmake_minimum_required(VERSION 3.10)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab01
        main.cpp
        src/parent.cpp 
        include/utils.h
        include/parent.h
        src/utils.cpp)

add_executable(lab01_child
        src/utils.cpp
        include/utils.h
        src/child.cpp)

target_include_directories(lab01 PRIVATE include)
target_include_directories(lab01_child PRIVATE include)

add_dependencies(lab01 lab01_child)