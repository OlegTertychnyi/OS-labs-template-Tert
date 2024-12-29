cmake_minimum_required(VERSION 3.20)

project(lab04)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(impl1 SHARED 
    src/translation_binary.cpp 
    src/sort_bubble.cpp
)
target_include_directories(impl1 PUBLIC include)
target_compile_definitions(impl1 PRIVATE LIBRARY_EXPORTS)

add_library(impl2 SHARED 
    src/translation_ternary.cpp 
    src/sort_quick.cpp
)
target_include_directories(impl2 PUBLIC include)
target_compile_definitions(impl2 PRIVATE LIBRARY_EXPORTS)

add_executable(program_compile_time src/main_compile_time.cpp)
target_link_libraries(program_compile_time PRIVATE impl1)

add_executable(program_runtime src/main_runtime.cpp)
target_include_directories(program_runtime PRIVATE include)

if(UNIX)
    target_link_libraries(program_runtime PRIVATE dl impl1 impl2)
else()
    target_link_libraries(program_runtime PRIVATE impl1 impl2)
endif()
