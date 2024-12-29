cmake_minimum_required(VERSION 3.10)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab03
        main.cpp
        src/parent.cpp 
        include/utils.h
        include/parent.h
        src/utils.cpp)

add_executable(child
        src/utils.cpp
        include/utils.h
        src/child.cpp)

target_include_directories(lab03 PRIVATE include)
target_include_directories(child PRIVATE include)

add_dependencies(lab03 child)