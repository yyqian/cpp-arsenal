#include "stdio.h"

struct SimpleTag {
  int a;
  char b;
};

typedef struct {
  int a;
  char b;
} Simple;

int main() {
  struct SimpleTag simpleTag;
  Simple simple;
  simpleTag.a = 3;
  simpleTag.b = 'a';
  simple.a = 4;
  simple.b = 'b';
  printf("%d|%d\n", simpleTag.a, simpleTag.b);
  printf("%d|%d\n", simple.a, simple.b);
  struct MyTag {
    int a;
    char b;
  } var;
  printf("%d|%d\n", var.a, var.b);
  return 0;
}