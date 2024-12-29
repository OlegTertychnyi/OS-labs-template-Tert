#pragma once
#include <vector>
#include <functional>

std::vector<int> ReadNumbers();
void WriteNumbersToMemory(char* mapped_memory, const std::vector<int>& numbers);
std::vector<int> ReadNumbersFromMemory(const char* mapped_memory);
int NumberSum(const std::vector<int>& numbers);
