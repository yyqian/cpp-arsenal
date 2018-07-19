#include <iostream>
#include <unistd.h>

using namespace std;

int z0;
int *y0;

void test() {
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
}

void race_cond() {
  pid_t pid;
  if ((pid = fork()) < 0) {
    cout << "error" << endl;
  } else if (0 == pid) {
    for (int i = 0; i < 2000; ++i) {
      cout << to_string(i) << "|";
    }
  } else {
    for (int i = 0; i < 2000; ++i) {
      cout << to_string(i) << "$";
    }
  }
  exit(0);
}

int main(int argc, char **argv) {
  race_cond();
  return 0;
}