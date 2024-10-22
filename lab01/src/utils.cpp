#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <functional>


void WriteNumbersToPipe(int pipefd, const std::vector<int>& numbers) {
    std::ostringstream oss;
    for (size_t i{0}; i < numbers.size(); ++i) {
        oss << numbers[i];
        if (i != numbers.size() - 1) {
            oss << " ";
        }
    }
    oss << "\n";

    std::string data = oss.str();
    write(pipefd, data.c_str(), data.size());
}

std::vector<int> ReadNumbers(){
    std::vector<int> numbers;
    int number;

    while (std::cin >> number) {
        numbers.push_back(number);

        if (std::cin.get() == '\n') {
            break;
        }
    }

    if (numbers.size() == 0){
        throw std::invalid_argument("Size of numbers cannot be a zero\n");
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

void ReadData(const std::function<void(const std::string&)>& handler, std::basic_istream<char>& stream){
    std::string buff;

    std::getline(stream, buff);
    if (buff.empty()){
        return;
    }
    handler(buff + '\n');
}