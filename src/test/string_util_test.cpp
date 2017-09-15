#include "gtest/gtest.h"
#include "string_util.h"
#ifdef _WIN32
#include "utf8.h"
#endif // _WIN32

using namespace std;

TEST(string, wchar) {
  wchar_t source0[] = L"A\u4E2D"; // UTF-16 encoded, size: 2 + 2 + 2 = 6
  wstring source1 = L"A\u4E2D"; // UTF-16 encoded
  char source2[] = u8"A\u4E2D"; // UTF-8 encoded, size: 1 + 3 + 1 = 5
  string source3 = u8"A\u4E2D"; // UTF-8 encoded
  string source4 = "A中"; //推荐的写法
#ifdef _WIN32
  EXPECT_EQ(2, sizeof(wchar_t));
  EXPECT_EQ(2 * 3, sizeof(source0));
  EXPECT_EQ(1 + 3 + 1, sizeof(source2));
  EXPECT_EQ("00 41 4E 2D", Hexify(source0));
  EXPECT_EQ("00 41 4E 2D", Hexify(source1.c_str()));
  EXPECT_EQ("41 E4 B8 AD", Hexify(source2));
  EXPECT_EQ("41 E4 B8 AD", Hexify(source3.c_str()));
  EXPECT_EQ("41 E4 B8 AD", Hexify(source4.c_str()));
  string source1_transform = Narrow(source1);
  wstring source3_transform = Widen(source3);
  EXPECT_EQ("41 E4 B8 AD", Hexify(source1_transform.c_str()));
  EXPECT_EQ("00 41 4E 2D", Hexify(source3_transform.c_str()));
#else
  EXPECT_EQ(4, sizeof(wchar_t));
  EXPECT_EQ(4 * 3, sizeof(source0));
  EXPECT_EQ(1 + 3 + 1, sizeof(source2));
  EXPECT_EQ("00 00 00 41 00 00 4E 2D", Hexify(source0));
  EXPECT_EQ("00 00 00 41 00 00 4E 2D", Hexify(source1.c_str()));
  EXPECT_EQ("41 E4 B8 AD", Hexify(source2));
  EXPECT_EQ("41 E4 B8 AD", Hexify(source3.c_str()));
  EXPECT_EQ("41 E4 B8 AD", Hexify(source4.c_str()));
#endif
}