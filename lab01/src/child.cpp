#include <iostream>
#include <fstream>
#include <unistd.h>
#include "utils.h"
#include <string>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Filename not provided" << std::endl;
        return 1;
    }
    
    FILE *file = fopen(argv[1], "w");

    dup2(fileno(file), STDOUT_FILENO);
    int sum{0};
    ReadData([&sum](const std::string& str) {
        std::istringstream iss(str);
        int num;
        while(iss >> num){
            sum += num;
        }
    }, std::cin);
    std::string str = std::to_string(sum) + '\n';
    write(STDOUT_FILENO, str.c_str(), str.size());

    fclose(file);
    close(STDOUT_FILENO);

    return 0;
}


