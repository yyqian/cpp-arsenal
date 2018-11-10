#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

void print_file(const char *filename) {
  struct stat f_stat;
  stat(filename, &f_stat);
  cout << "birth: " << endl;
  if (S_ISCHR(f_stat.st_mode)) {
    cout << "/dev/null is char special file" << endl;
  }
}

int main(int argc, char **argv) {
  cout << "Hello" << endl;
  print_file("/dev/null");
  if(link("/Users/yyqian/tst", "/Users/yyqian/tst_lnk") != 0) {
    cout << "link failed" << endl;
  }
  //unlink("/Users/yyqian/ss");
  symlink("/Users/yyqian/ss", "/Users/yyqian/ss_lnk");
  char *buf = new char[100];
  readlink("/Users/yyqian/ss_lnk", buf, 100);
  cout << buf << endl;
  delete[] buf;
  return 0;
}