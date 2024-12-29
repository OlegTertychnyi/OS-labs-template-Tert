#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include "utils.h"

int main() {

    const char* filename = getenv("FILENAME");
    const char* mmapFile = getenv("MMAP_FILE");

    if (!filename || !mmapFile) {
        std::cerr << "Environment variables FILENAME or MMAP_FILE not set" << std::endl;
        return 1;
    }

    const size_t MMAP_SIZE = 4096;

    int fd = open(mmapFile, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    void* addr = mmap(nullptr, MMAP_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    std::vector<int> numbers = ReadNumbersFromMemory(static_cast<char*>(addr));
    int sum = NumberSum(numbers);

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        munmap(addr, MMAP_SIZE);
        close(fd);
        return 1;
    }

    file << sum << std::endl;

    munmap(addr, MMAP_SIZE);
    close(fd);

    return 0;
}
