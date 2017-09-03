#pragma once
#include <vector>

// fit: y = sum_i{a_i * x^i}
class Fit {
  std::vector<double> factors;
public:
  template<typename T>
  void LinearFit(const std::vector<T> &x, const std::vector<T> &y);

  std::vector<double> get_factors() {
    return this->factors;
  }

  double get_intercept() {
    return factors[0];
  }

  double get_slope() {
    return factors[1];
  }
};

double Slope(const std::vector<int> &x, const std::vector<int> &y);
