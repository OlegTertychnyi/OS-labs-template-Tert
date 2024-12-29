#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <sstream>
#include <vector>
#include <filesystem>
#include "utils.h"
#include "parent.h"

int ParentMain(std::istream& input) {
    const std::string mmapFile = std::filesystem::absolute("/tmp/mmap_memory").string();
    const size_t MMAP_SIZE = 4096;

    int fd = open(mmapFile.c_str(), O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    if (ftruncate(fd, MMAP_SIZE) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    void* addr = mmap(nullptr, MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    std::string filename;
    std::getline(input, filename);

    filename = std::filesystem::absolute(filename).string();

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        munmap(addr, MMAP_SIZE);
        close(fd);
        return 1;
    }

    if (pid == 0) {
        munmap(addr, MMAP_SIZE);
        close(fd);

        const std::string childPath = std::filesystem::absolute("./lab03/child").string();

        setenv("FILENAME", filename.c_str(), 1);
        setenv("MMAP_FILE", mmapFile.c_str(), 1);
        execl(childPath.c_str(), "child", nullptr);
        perror("execl");
        return 1;
    }

    std::vector<int> numbers;
    int num;
    while (input >> num) {
        numbers.push_back(num);
    }

    WriteNumbersToMemory(static_cast<char*>(addr), numbers);

    wait(nullptr);

    munmap(addr, MMAP_SIZE);
    close(fd);
    unlink(mmapFile.c_str());

    return 0;
}