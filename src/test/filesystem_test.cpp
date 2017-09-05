//
// Created by Yinyin Qian on 8/18/17.
//
#include "gtest/gtest.h"
#include "filesystem.h"

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
