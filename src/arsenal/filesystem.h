#pragma once
#include <string>
#include <vector>

namespace arsenal {
enum Encoding {
  UNKNOWN_ENCODING = 0,
  UTF_8 = 1,
  UTF_16_BE = 2,
  UTF_16_LE = 3,
  UTF_32_BE = 4,
  UTF_32_LE = 5
};
}

void ListFiles(const std::string &dir_name, std::vector<std::string> &files,
               bool recur = true);

arsenal::Encoding HasBom(const std::string &filename);

inline std::string GetExtension(const std::string &filename) {
  return filename.substr(filename.find_last_of('.') + 1);
}
