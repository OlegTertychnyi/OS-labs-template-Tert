cmake_minimum_required(VERSION 3.20)
project(OS_LABS C CXX)

enable_testing()

add_compile_options(-Wall -Wextra -pedantic -Werror)

find_package(Threads REQUIRED)

# Уже существующие лабы
add_subdirectory(lab01)
add_subdirectory(lab02)
add_subdirectory(lab03)
add_subdirectory(lab04)

# Поддиректория с нашим заданием 5–7
add_subdirectory(lab57)

# Поддиректория с тестами
add_subdirectory(tests)
