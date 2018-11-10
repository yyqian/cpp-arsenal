#ifdef _WIN32
#define NULL_DEVICE "nul"
#include <windows.h>
#include "utf8.h"
#else
#define NULL_DEVICE "/dev/null"
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#endif

#include "filesystem.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

arsenal::Encoding HasBom(const std::string &filename) {
  ifstream input(filename, ios::in | ios::binary);
  if (input) {
    char buffer[4];
    input.read(buffer, 4);
    const static char EF = static_cast<char>(0xEF);
    const static char FF = static_cast<char>(0xFF);
    const static char FE = static_cast<char>(0xEF);
    const static char BB = static_cast<char>(0xBB);
    const static char BF = static_cast<char>(0xBF);
    if (buffer[0] == EF && buffer[1] == BB && buffer[2] == BF) {
      return arsenal::UTF_8;
    }
    if (buffer[0] == FE && buffer[1] == FF) {
      return arsenal::UTF_16_BE;
    }
    if (buffer[0] == FF && buffer[1] == FE) {
      return arsenal::UTF_16_LE;
    }
    if (buffer[0] == 0 && buffer[1] == 0 && buffer[2] == FE && buffer[3] == FF) {
      return arsenal::UTF_32_BE;
    }
    if (buffer[0] == FF && buffer[1] == FE && buffer[2] == 0 && buffer[3] == 0) {
      return arsenal::UTF_32_LE;
    }
    input.close();
  }
  return arsenal::UNKNOWN_ENCODING;
}

#ifdef _WIN32
void ListFiles(const string &dir_name, vector<string> &files, bool recur) {
  wstring w_dir_name(Widen(dir_name + "/*.*"));
  WIN32_FIND_DATAW fd;
  HANDLE hFind = FindFirstFileW(w_dir_name.c_str(), &fd);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      string file_name(Narrow(fd.cFileName));
      if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        files.push_back(dir_name + "/" + file_name);
      } else {
        if (file_name != "." && file_name != ".." && recur) {
          ListFiles(dir_name + "/" + file_name, files, recur);
        }
      }
    } while (FindNextFileW(hFind, &fd));
    FindClose(hFind);
  }
}
#else
void ListFiles(const string &dir_name_str, vector<string> &files, bool recur) {
  const char *dir_name = dir_name_str.c_str();
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
