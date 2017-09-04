#pragma once
#include <string>
#include <vector>

void ListFiles(const std::string &dir_name, std::vector<std::string> &files, bool recur = true);
