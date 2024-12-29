#include "library.h"
#include <string>
#include <cstring>
#include <bitset>

extern "C" {

LIBRARY_API char* translation(long x) {
    std::bitset<64> binary(x);
    std::string binaryStr = binary.to_string();
    size_t first1 = binaryStr.find('1');
    if (first1 != std::string::npos) {
        binaryStr = binaryStr.substr(first1);
    } else {
        binaryStr = "0";
    }
    char* result = new char[binaryStr.length() + 1];
    std::strcpy(result, binaryStr.c_str());
    return result;
}

}
