#pragma once
#include <numeric>
#include <vector>

// fit: y = sum_i{a_i * x^i}
class Fit {
  std::vector<double> factors;

 public:
  template <typename T>
  void LinearFit(const std::vector<T> &x, const std::vector<T> &y) {
    factors.resize(2, 0.0);
    auto len = x.size() > y.size() ? y.size() : x.size();
    double x_sum = 0;
    double y_sum = 0;
    double xy_sum = 0;
    double xx_sum = 0;
    for (decltype(len) i = 0; i != len; ++i) {
      x_sum += x[i];
      y_sum += y[i];
      xy_sum += x[i] * y[i];
      xx_sum += x[i] * x[i];
    }
    this->factors[0] =
        (xx_sum * y_sum - x_sum * xy_sum) / (len * xx_sum - x_sum * x_sum);
    this->factors[1] =
        (len * xy_sum - x_sum * y_sum) / (len * xx_sum - x_sum * x_sum);
  }

  std::vector<double> get_factors() { return this->factors; }

  double get_intercept() { return factors[0]; }

  double get_slope() { return factors[1]; }
};

template <typename T>
double Slope(const std::vector<T> &x, const std::vector<T> &y) {
  const auto n = x.size();
  const double s_x = accumulate(x.begin(), x.end(), 0.0);
  const double s_y = accumulate(y.begin(), y.end(), 0.0);
  const double s_xx = inner_product(x.begin(), x.end(), x.begin(), 0.0);
  const double s_xy = inner_product(x.begin(), x.end(), y.begin(), 0.0);
  return (n * s_xy - s_x * s_y) / (n * s_xx - s_x * s_x);
}
