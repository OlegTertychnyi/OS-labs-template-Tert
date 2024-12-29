#include "library.h"
#include <string>
#include <cstring>

extern "C" {

LIBRARY_API char* translation(long x) {
    if (x == 0) {
        char* zero = new char[2];
        strcpy(zero, "0");
        return zero;
    }

    std::string ternary = "";
    long num = x;
    bool negative = false;
    if (num < 0) {
        negative = true;
        num = -num;
    }

    while (num > 0) {
        ternary = std::to_string(num % 3) + ternary;
        num /= 3;
    }

    if (negative) {
        ternary = "-" + ternary;
    }

    char* result = new char[ternary.length() + 1];
    std::strcpy(result, ternary.c_str());
    return result;
}

}
