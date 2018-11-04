#include <omp.h>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>

int main(int argc, char **argv) {
#pragma omp parallel for
  for (char i = 'a'; i <= 'z'; ++i) {
    std::stringstream ss;
    ss << '[' << std::this_thread::get_id() << ']' << i << "\n";
    std::cout << ss.str();
  }
  return 0;
}