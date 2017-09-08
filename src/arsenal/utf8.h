#pragma once
#include <codecvt>
// convert UTF-8 string to wstring(UTF-16)
inline std::wstring Widen(const std::string& str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.from_bytes(str);
}

inline std::wstring Widen(const char *str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.from_bytes(str);
}

// convert wstring(UTF-16) to UTF-8 string
inline std::string Narrow(const std::wstring& str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.to_bytes(str);
}

inline std::string Narrow(const wchar_t *str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.to_bytes(str);
}
