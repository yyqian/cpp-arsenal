#include "gtest/gtest.h"
#include "fit.h"

using namespace std;

TEST(fit, Slope) {
  vector<int> x = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  vector<int> y(x.size());
  const int actual_slope = 2;
  transform(x.begin(), x.end(), y.begin(), [actual_slope](int i) {
    return actual_slope * i;
  });
  double slope = Slope(x, y);
  EXPECT_EQ(actual_slope, (int)slope);
}
