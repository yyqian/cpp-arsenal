#include "filesystem.h"
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "Usage: bom-check /path/to/dir" << endl;
    return 1;
  }
  vector<string> files;
  ListFiles(argv[1], files, true);
  vector<string> ignore_extension_list;
  for (int i = 2; i < argc; ++i) {
    ignore_extension_list.push_back(argv[i]);
  }
  for (const string &str : files) {
    const string ext = GetExtension(str);
    bool ignore = false;
    for (const string &str : ignore_extension_list) {
      if (str == ext) {
        ignore = true;
        break;
      }
    }
    if (ignore) {
      continue;
    }
    arsenal::Encoding encoding = HasBom(str);
    switch (encoding) {
    case arsenal::UTF_8:
      cout << "[UTF-8-BOM]" << str << endl;
      break;
    case arsenal::UTF_16_BE:
      cout << "[UTF-16-BE]" << str << endl;
      break;
    case arsenal::UTF_16_LE:
      cout << "[UTF-16-LE]" << str << endl;
      break;
    case arsenal::UTF_32_BE:
      cout << "[UTF-32-BE]" << str << endl;
      break;
    case arsenal::UTF_32_LE:
      cout << "[UTF-32-LE]" << str << endl;
      break;
    }
  }
  return 0;
}
