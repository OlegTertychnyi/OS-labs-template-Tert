#include "library.h"
#include <iostream>
#include <vector>
#include <cstring>

int main() {
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

        if(tokens[0] == "1") {
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

    return 0;
}
