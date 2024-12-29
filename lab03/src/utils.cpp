#include "utils.h"
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <iostream>

std::vector<int> ReadNumbers() {
    std::vector<int> numbers;
    int number;

    while (std::cin >> number) {
        numbers.push_back(number);
        if (std::cin.get() == '\n') {
            break;
        }
    }

    if (numbers.empty()) {
        throw std::invalid_argument("Size of numbers cannot be zero\n");
    }
    return numbers;
}

void WriteNumbersToMemory(char* mappedMemory, const std::vector<int>& numbers) {
    std::ostringstream oss;
    for (size_t i = 0; i < numbers.size(); ++i) {
        oss << numbers[i];
        if (i != numbers.size() - 1) {
            oss << " ";
        }
    }
    std::string data = oss.str();
    std::memcpy(mappedMemory, data.c_str(), data.size());
}

std::vector<int> ReadNumbersFromMemory(const char* mappedMemory) {
    std::vector<int> numbers;
    std::istringstream iss(mappedMemory);
    int num;
    while (iss >> num) {
        numbers.push_back(num);
    }
    return numbers;
}

int NumberSum(const std::vector<int>& numbers) {
    int sum = 0;
    for (int num : numbers) {
        sum += num;
    }
    return sum;
}
