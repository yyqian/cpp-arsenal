//
// Created by Yinyin Qian on 8/18/17.
//

#include "gtest/gtest.h"
#include "filesystem.h"
#include <algorithm>

using namespace std;

TEST(filesystem, ListFiles) {
  vector<string> files;
  const string path(u8"C:/Users/yyqian/Downloads/\u8F6F\u4EF6");
  cout << path << endl;
  ListFiles(path, files, true);
  for_each(files.cbegin(), files.cend(), [](const string &str) {
    cout << str << endl;
  });
}

TEST(filesystem, CheckBom) {
  vector<string> files;
  const string path(u8"D:/source/repos/cpp-arsenal");
  ListFiles(path, files, true);
  for_each(files.cbegin(), files.cend(), [](const string &str) {
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
  });
}

