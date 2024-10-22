#pragma once
#include <vector>
#include <functional>

#ifndef OS_LABS_PARENT_H
#define OS_LABS_PARENT_H

std::vector<int> ReadNumbers();
void WriteNumbersToPipe(int pipefd, const std::vector<int>& numbers);
int NumberSum(const std::vector<int>& numbers);
void ReadData(const std::function<void(const std::string&)>& handler, std::basic_istream<char>& stream);

#endif