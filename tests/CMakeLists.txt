include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


set(CMAKE_CXX_STANDARD 20)

add_executable(lab1_test lab1_test.cpp 
              ../lab01/src/parent.cpp ../lab01/include/parent.h
              ../lab01/src/utils.cpp ../lab01/include/utils.h)

target_link_libraries(lab1_test PRIVATE GTest::GTest)
target_include_directories(lab1_test PRIVATE ../lab01/include)

add_test(NAME lab1_test COMMAND lab1_test)