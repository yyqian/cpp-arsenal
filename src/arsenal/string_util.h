#pragma once
#include <string>

std::string Hexify(const wchar_t *c);

std::string Hexify(const char *c);

int ActualLength(const char *c);

void Trim(std::string &s);

void RemoveSpace(std::string &s);

std::wstring Utf8ToWstring(const std::string& str);

std::string WstringToUtf8(const std::wstring& str);
