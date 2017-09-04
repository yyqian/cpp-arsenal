﻿//
// Created by Yinyin Qian on 8/18/17.
//

#include "gtest/gtest.h"
#include "filesystem.h"
#include <algorithm>

using namespace std;

TEST(filesystem, ListFiles) {
  vector<string> files;
  const string path(u8"C:/Users/yyqian/Downloads/软件");
  cout << path << endl;
  ListFiles(path, files, true);
  for_each(files.cbegin(), files.cend(), [](const string &str) {
    cout << str << endl;
  });
}
