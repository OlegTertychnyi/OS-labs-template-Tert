cmake_minimum_required(VERSION 3.20)

include(FetchContent)

# Подтягиваем GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Поиск ZeroMQ
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)
find_package(Threads REQUIRED)

# Параметры для других библиотек, если необходимо
set(LIBIMPL1_PATH "${CMAKE_BINARY_DIR}/lib/libimpl1.so")
set(LIBIMPL2_PATH "${CMAKE_BINARY_DIR}/lib/libimpl2.so")
set(MAIN_RUNTIME_PATH "${CMAKE_BINARY_DIR}/bin/program_runtime")
set(MAIN_COMPILE_TIME_PATH "${CMAKE_BINARY_DIR}/bin/program_compile_time")

# Тесты для lab01
add_executable(lab1_test lab1_test.cpp 
              ../lab01/src/parent.cpp ../lab01/include/parent.h
              ../lab01/src/utils.cpp ../lab01/include/utils.h)

target_link_libraries(lab1_test PRIVATE GTest::gtest_main)

target_include_directories(lab1_test PRIVATE ../lab01/include)

add_test(NAME lab1_test COMMAND lab1_test)

# Тесты для lab02
add_executable(lab2_test lab2_test.cpp 
              ../lab02/src/gauss.cpp ../lab02/include/gauss.h)

target_link_libraries(lab2_test PRIVATE GTest::gtest_main)

target_include_directories(lab2_test PRIVATE ../lab02/include)

add_test(NAME lab2_test COMMAND lab2_test)

# Тесты для lab03
add_executable(lab3_test lab3_test.cpp 
              ../lab03/src/parent.cpp ../lab03/include/parent.h
              ../lab03/src/utils.cpp ../lab03/include/utils.h)

target_link_libraries(lab3_test PRIVATE GTest::gtest_main)

target_include_directories(lab3_test PRIVATE ../lab03/include)

add_test(NAME lab3_test COMMAND lab3_test)

# Тесты для lab04
add_executable(lab4_test lab4_test.cpp)

target_link_libraries(lab4_test PRIVATE GTest::gtest_main dl impl1 impl2)

target_include_directories(lab4_test PRIVATE ../lab04/include)

target_compile_definitions(lab4_test PRIVATE 
    LIBIMPL1_PATH="${LIBIMPL1_PATH}" 
    LIBIMPL2_PATH="${LIBIMPL2_PATH}"
)

set_target_properties(lab4_test PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(lab4_test PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")

add_test(NAME lab4_test COMMAND lab4_test)

add_executable(lab57_test
    lab57_test.cpp
)

target_include_directories(lab57_test PRIVATE
    ../lab57/include
    ${ZMQ_INCLUDE_DIRS}
)

target_link_libraries(lab57_test PRIVATE
    GTest::gtest_main
    lab57_lib
)

add_test(NAME lab57_test COMMAND lab57_test)