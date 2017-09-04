#include "str.h"
#include <sstream>
#include <algorithm>
#include <codecvt>

using namespace std;

// convert C style string to hex string
void HexCStr(const char *c, std::string &str) {
    const static char dict[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    ostringstream ss;
    bool first = true;
    do {
        if (first) {
            first = false;
        }
        else {
            ss << ' ';
        }
        ss << dict[(*c >> 4) & 0xF];
        ss << dict[*c & 0xF];
    } while (*(++c) != 0); // C style string end with 0x00 or '\0'
    str = ss.str();
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

#ifdef _WIN32
// convert UTF-16 string array to wstring
wstring Char2Wstring(const char *input) {
  const size_t newsize = strlen(input) + 1;
  wchar_t *output = new wchar_t[newsize];
  size_t converted_chars = 0;
  mbstowcs_s(&converted_chars, output, newsize, input, _TRUNCATE);
  wstring wstr(output);
  delete[] output;
  return wstr;
}

// convert wstring to UTF-16 string
string Wchar2String(const wchar_t *input) {
  const size_t newsize = (wcslen(input) + 1) * 2;
  char *output = new char[newsize];
  size_t convertedChars = 0;
  wcstombs_s(&convertedChars, output, newsize, input, _TRUNCATE);
  string str(output);
  delete[] output;
  return str;
}
#endif

// convert UTF-8 string to wstring(UTF-16)
std::wstring Utf8ToWstring(const std::string& str)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.from_bytes(str);
}

// convert wstring(UTF-16) to UTF-8 string
std::string WstringToUtf8(const std::wstring& str)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.to_bytes(str);
}
