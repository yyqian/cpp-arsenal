#include "gtest/gtest.h"
#include "string_util.h"

using namespace std;

TEST(string, wchar) {
  wchar_t source0[] = L"A\u4E2D"; // UTF-16 encoded, size: 2 + 2 + 2 = 6
  wstring source1 = L"A\u4E2D"; // UTF-16 encoded
  char source2[] = u8"A\u4E2D"; // UTF-8 encoded, size: 1 + 3 + 1 = 5
  string source3 = u8"A\u4E2D"; // UTF-8 encoded
  EXPECT_EQ(6, sizeof(source0));
  EXPECT_EQ(5, sizeof(source2));
  EXPECT_EQ("00 41 4E 2D", Hexify(source0));
  EXPECT_EQ("00 41 4E 2D", Hexify(source1.c_str()));
  EXPECT_EQ("41 E4 B8 AD", Hexify(source2));
  EXPECT_EQ("41 E4 B8 AD", Hexify(source3.c_str()));
}