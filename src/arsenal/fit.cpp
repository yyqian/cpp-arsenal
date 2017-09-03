#include "fit.h"
#include <numeric>

using namespace std;

template<typename T>
void Fit::LinearFit(const vector<T> &x, const vector<T> &y) {
  factors.resize(2, 0.0);
  auto len = x.size() > y.size() ? y.size() : x.size();
  T x_sum = 0;
  T y_sum = 0;
  T xy_sum = 0;
  T xx_sum = 0;
  for (decltype(len) i = 0; i != len; ++i) {
    x_sum += x[i];
    y_sum += y[i];
    xy_sum += x[i] * y[i];
    xx_sum += x[i] * x[i];
  }
  this->factors[0] = static_cast<double>(xx_sum * y_sum - x_sum * xy_sum)
    / (len * xx_sum - x_sum * x_sum);
  this->factors[1] = static_cast<double>(len * xy_sum - x_sum * y_sum)
    / (len * xx_sum - x_sum * x_sum);
}

double Slope(const vector<int> &x, const vector<int> &y) {
  const auto n = x.size();
  const long s_x = accumulate(x.begin(), x.end(), 0L);
  const long s_y = accumulate(y.begin(), y.end(), 0L);
  const long s_xx = inner_product(x.begin(), x.end(), x.begin(), 0L);
  const long s_xy = inner_product(x.begin(), x.end(), y.begin(), 0L);
  return static_cast<double>(n * s_xy - s_x * s_y) / (n * s_xx - s_x * s_x);
}
