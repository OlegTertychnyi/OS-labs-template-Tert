#include "library.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include <dlfcn.h>

typedef char* (*translation_func)(long);
typedef int* (*sort_func)(int*, int);

int main() {
    std::string current_library = "./libimpl1.so";

    translation_func translation = nullptr;
    sort_func Sort = nullptr;

    auto load_library = [&](const std::string& lib_path) -> void* {
        void* handle = dlopen(lib_path.c_str(), RTLD_LAZY);
        if(!handle) {
            std::cerr << "Не удалось загрузить библиотеку: " << lib_path << "\n";
            std::cerr << "Ошибка: " << dlerror() << "\n";
        }
        return handle;
    };

    auto unload_library = [&](void* handle) -> void {
        if(!handle) return;
        dlclose(handle);
    };

    auto get_function = [&](void* handle, const char* func_name) -> void* {
        if(!handle) return nullptr;
        return dlsym(handle, func_name);
    };

    void* lib_handle = load_library(current_library);
    if(!lib_handle) {
        return 1;
    }

    translation = (translation_func)get_function(lib_handle, "translation");
    Sort = (sort_func)get_function(lib_handle, "Sort");

    if(!translation || !Sort) {
        std::cerr << "Не удалось найти необходимые функции в библиотеке\n";
        unload_library(lib_handle);
        return 1;
    }

    std::cout << "Используется библиотека: " << current_library << "\n";

    while(true) {
        std::string input;
        std::cout << "Введите команду: ";
        std::getline(std::cin, input);
        if(input.empty()) continue;

        std::vector<std::string> tokens;
        size_t pos = 0;
        while((pos = input.find(' ')) != std::string::npos) {
            tokens.emplace_back(input.substr(0, pos));
            input.erase(0, pos +1);
        }
        tokens.emplace_back(input);

        if(tokens.empty()) continue;

        if(tokens[0] == "0") {
            unload_library(lib_handle);
            if(current_library == "./libimpl1.so") {
                current_library = "./libimpl2.so";
            }
            else {
                current_library = "./libimpl1.so";
            }
            lib_handle = load_library(current_library);
            if(!lib_handle) {
                std::cerr << "Не удалось переключить библиотеку\n";
                return 1;
            }
            translation = (translation_func)get_function(lib_handle, "translation");
            Sort = (sort_func)get_function(lib_handle, "Sort");
            if(!translation || !Sort) {
                std::cerr << "Не удалось найти функции в новой библиотеке\n";
                unload_library(lib_handle);
                return 1;
            }
            std::cout << "Переключено на библиотеку: " << current_library << "\n";
        }
        else if(tokens[0] == "1") {
            if(tokens.size() < 2) {
                std::cerr << "Недостаточно аргументов для функции 1\n";
                continue;
            }
            long x = std::stol(tokens[1]);
            char* result = translation(x);
            std::cout << "translation(" << x << ") = " << result << "\n";
            delete[] result;
        }
        else if(tokens[0] == "2") {
            if(tokens.size() < 2) {
                std::cerr << "Недостаточно аргументов для функции 2\n";
                continue;
            }
            std::vector<int> array;
            for(size_t i =1; i < tokens.size(); ++i) {
                array.push_back(std::stoi(tokens[i]));
            }
            int* sorted = Sort(array.data(), array.size());
            std::cout << "Sorted array: ";
            for(size_t i =0; i < array.size(); ++i) {
                std::cout << sorted[i] << " ";
            }
            std::cout << "\n";
        }
        else {
            std::cerr << "Неизвестная команда\n";
        }
    }

    unload_library(lib_handle);
    return 0;
}
