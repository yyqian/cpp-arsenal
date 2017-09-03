#include "str.h"
#include <sstream>
#include <iostream>
#include <algorithm>

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