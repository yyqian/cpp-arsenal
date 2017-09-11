#include "gtest/gtest.h"
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32


GTEST_API_ int main(int argc, char **argv) {
#ifdef _WIN32
  SetConsoleOutputCP(65001); // Set console output code page to 65001(UTF-8)
#endif // _WIN32
  std::cout << u8"====== Running Unit Test | \u5F00\u59CB\u5355\u5143\u6D4B\u8BD5 ======" << std::endl;
  testing::InitGoogleTest(&argc, argv);
  int code = RUN_ALL_TESTS();
#ifdef _WIN32
  std::cout << "Press Enter to continue...";
  getchar();
#endif // _WIN32
  return code;
}
