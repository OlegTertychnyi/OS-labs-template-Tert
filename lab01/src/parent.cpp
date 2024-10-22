#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/wait.h>
#include "utils.h"
#include <sstream>


int ParentMain(std::istream& input) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    std::string filename;
    std::cout << "Enter filename: ";
    std::getline(input, filename);
    std::cout << "FILES: " << filename << std::endl;
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Fork failed" << std::endl;
        return 1;
    }

    if (pid == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        execl("./lab01/child", "child", filename.c_str(), nullptr);
        std::cerr << "Failed to execute child process" << std::endl;
        close(STDIN_FILENO);
        return 1;

    }
    close(pipefd[0]);
    std::vector<int> numbers;    
    ReadData([&numbers](const std::string& str) {
        std::istringstream iss(str);
        int num;
        while(iss >> num){
            numbers.push_back(num);
        }
    }, input);
    WriteNumbersToPipe(pipefd[1], numbers);
    close(pipefd[1]);

    wait(nullptr);

    close(STDIN_FILENO);

    return 0;
}
