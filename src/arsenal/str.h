#pragma once
#include <string>
#include <vector>

void HexCStr(const char *c, std::string &str);

int ActualLength(const char *c);

void Trim(std::string &s);

void RemoveSpace(std::string &s);

std::wstring Utf8ToWstring(const std::string& str);

std::string WstringToUtf8(const std::wstring& str);
