#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;
int main(int argc, char **argv) {
  cout << "Hello" << endl;
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