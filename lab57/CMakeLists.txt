cmake_minimum_required(VERSION 3.10)
project(lab57 C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include ${ZMQ_INCLUDE_DIRS})

add_library(lab57_lib
    src/manager.cpp
    src/utils.cpp
    include/manager.h
    include/utils.h
)

target_link_libraries(lab57_lib
    PUBLIC
    ${ZMQ_LIBRARIES}
    Threads::Threads
)

add_executable(lab57
    main.cpp
)
target_link_libraries(lab57 PRIVATE lab57_lib)

add_executable(compute_node
    src/compute_node.cpp
    include/compute_node.h
)

target_link_libraries(compute_node PRIVATE lab57_lib)


set_target_properties(compute_node PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../tests"
)

target_compile_definitions(
    lab57_lib
    PUBLIC
    COMPUTE_NODE_PATH="$<TARGET_FILE:compute_node>"
)
