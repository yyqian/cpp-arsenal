//
// Created by Yinyin Qian on 8/18/17.
//

#include "gtest/gtest.h"
//#include "filesystem.h"
#include <algorithm>

#include <windows.h>
#include <tchar.h>

using namespace std;

TEST(load, ListFiles) {
  string wstr("E:/Softwares/*.*");
  vector<string> fileArray;
  WIN32_FIND_DATA ffd;
  HANDLE hFind = FindFirstFile(wstr.c_str(), &ffd);
  if (hFind != INVALID_HANDLE_VALUE)
  {
    do {
      if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
        fileArray.push_back(ffd.cFileName);
    } while (FindNextFile(hFind, &ffd));
    FindClose(hFind);
  }
  for_each(fileArray.cbegin(), fileArray.cend(), [](const string &ws) {
    cout << ws << endl;
  });
}
