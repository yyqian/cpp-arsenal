#include <iostream>

int z0;
int *y0;

int main(int argc, char **argv) {
  int i, *j;   // bad
  int *x, y;   // bad
  int *m, *n;  // bad
  int *k;
  char **a;
  i = 1234;
  j = &i;
  y = 56789;
  x = &y;
  std::cout << "hello, world!" << i << *j << *x << y << std::endl;
  int z1;
  int *y1;
  std::cout << "z0 has default value:" << z0 << ", z1 is uninitialized:" << z1
            << std::endl;
  std::cout << "y0 has default value:" << y0 << ", y1 is uninitialized:" << y1
            << std::endl;
  return 0;
}