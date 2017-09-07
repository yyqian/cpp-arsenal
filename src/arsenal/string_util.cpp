#include "string_util.h"
#include <sstream>
#include <algorithm>
#include <codecvt>

using namespace std;

// convert C style string to hex string
std::string Hexify(const char *c) {
    static const char dict[] = "0123456789ABCDEF";
    ostringstream ss;
    bool first = true;
    do {
        if (first) {
            first = false;
        } else {
            ss << ' ';
        }
        ss << dict[(*c >> 4) & 0xF];
        ss << dict[*c & 0xF];
    } while (*(++c) != 0); // C style string end with 0x00 or '\0'
    return ss.str();
}

std::string Hexify(const wchar_t *wc)
{
  static const char dict[] = "0123456789ABCDEF";
  static const int len = static_cast<int>(sizeof(wchar_t));
  ostringstream ss;
  bool first = true;
  do {
    if (first) {
      first = false;
    } else {
      ss << ' ';
    }
    int shift = 8 * (len - 1);
    ss << dict[(*wc >> (shift + 4)) & 0xF];
    ss << dict[(*wc >> shift) & 0xF];
    for (int i = len - 2; i >= 0; --i) {
      shift = 8 * i;
      ss << ' ';
      ss << dict[(*wc >> (shift + 4)) & 0xF];
      ss << dict[(*wc >> shift) & 0xF];
    }
  } while (*(++wc) != 0);
  return ss.str();
}

// count actual number of characters, '\0' excluded
int ActualLength(const char *c) {
    int i = 0;
    while (*(c++) != 0) {
        ++i;
    }
    return i;
}

void Trim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
        return 0 == isspace(ch);
    }));
    s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
        return 0 == isspace(ch);
    }).base(), s.end());
}

void RemoveSpace(string &s) {
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
}

// convert UTF-8 string to wstring(UTF-16)
std::wstring Utf8ToWstring(const std::string& str)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.from_bytes(str);
}

// convert wstring(UTF-16) to UTF-8 string
std::string WstringToUtf8(const std::wstring& str)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.to_bytes(str);
}
