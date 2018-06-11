#include <iostream>

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
  return 0;
}