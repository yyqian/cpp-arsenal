#include <iostream>
#include <string>
#include <memory>

using namespace std;

struct Base {
  int x;
  Base(int v): x{v} {
    cout  << "Base()\n";
  }
  ~Base() {
    cout << "~Base()\n"; // non-virtual destructor is OK here
  }
};

struct Derived: public Base {
  int y;
  Derived(int xV, int yV): Base(xV), y{yV} {
    cout  << "Derived()\n";
  }
  ~Derived() {
    cout << "~Derived()\n"; // non-virtual destructor is OK here
  }
};

shared_ptr<Base> generate() {
  return make_shared<Derived>(10, 20);
}

void consume() {
  shared_ptr<Base> p = generate();
  cout << p.use_count() << endl;
  cout << p->x << endl;
}


int main(int argc, char **argv) {
  cout << "begin consume\n";
  consume();
  cout << "after consume\n";
  return 0;
}