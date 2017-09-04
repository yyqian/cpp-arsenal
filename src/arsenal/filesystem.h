#pragma once
#include <string>
#include <vector>

void ListFiles(const char *dir_name, std::vector<std::string> &files, bool recur = true);
