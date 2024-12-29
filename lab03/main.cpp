#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "parent.h"

int main() {
    return ParentMain(std::cin);
}