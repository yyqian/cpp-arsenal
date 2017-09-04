#ifdef _WIN32
#define NULL_DEVICE "nul"
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <clocale>
#else
#define NULL_DEVICE "/dev/null"
#include <dirent.h>
#include <sys/stat.h>
#endif

#include "filesystem.h"
#include <iostream>
#include <string>

using namespace std;

#ifdef _WIN32
wstring Char2WString(const char *input) {
  const size_t newsize = strlen(input) + 1;
  wchar_t *output = new wchar_t[newsize];
  size_t converted_chars = 0;
  mbstowcs_s(&converted_chars, output, newsize, input, _TRUNCATE);
  wstring wstr(output);
  delete[] output;
  return wstr;
}

string WChar2String(const wchar_t *input) {
  const size_t newsize = (wcslen(input) + 1) * 2;
  char *output = new char[newsize];
  size_t convertedChars = 0;
  wcstombs_s(&convertedChars, output, newsize, input, _TRUNCATE);
  string str(output);
  delete[] output;
  return str;
}

void ListFiles(const char *dir_name, vector<string> &files, bool recur) {
  wstring w_dir_name = Char2WString(dir_name);
  w_dir_name += L"/*.*";
  wcout << w_dir_name << endl;
  cout << WChar2String(w_dir_name.c_str()) << endl;
  WIN32_FIND_DATAW fd;
  HANDLE hFind = FindFirstFileW(w_dir_name.c_str(), &fd);
  wcout << L"<" << fd.cFileName << L">" << endl;
  string file_name = WChar2String(fd.cFileName);
  cout << ">" << file_name << "<" << endl;
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      // read all (real) files in current folder
      // , delete '!' read other 2 default folder . and ..
      if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        string fullname(dir_name);
        fullname = (fullname + "/") + file_name;
        files.push_back(fullname);
      }
      else {
        string basename(file_name);
        if (basename != "." && basename != ".." && recur) {
          string fullname(dir_name + ("/" + basename));
          ListFiles(fullname.c_str(), files, recur);
        }
      }
    } while (FindNextFileW(hFind, &fd));
    FindClose(hFind); //Close the handle after use or memory/resource leak
  }
}
#else
void ListFiles(const char *dir_name, vector<string> &files, bool recur) {
  struct dirent *dir;
  DIR *d = opendir(dir_name);
  if (d) {
    while ((dir = readdir(d)) != nullptr) {
      auto &type = dir->d_type;
      auto &name = dir->d_name;
      if (type != DT_DIR && strcmp(name, ".DS_Store") != 0) {
        char full_name[1024];
        strcpy(full_name, dir_name);
        strcat(full_name, "/");
        strcat(full_name, name);
        files.push_back(string(full_name));
      }
      else if (DT_DIR == type && strcmp(name, ".") != 0 && strcmp(name, "..") != 0 && recur) {
        char sub_dir_name[1024];
        strcpy(sub_dir_name, dir_name);
        strcat(sub_dir_name, "/");
        strcat(sub_dir_name, name);
        ListFiles(sub_dir_name, files, recur);
      }
    }
    closedir(d);
  }
}
#endif
