#include <memory>
#include <iostream>
#include <string>
#include <list>
#include <vector>

using namespace std;

struct Base {
  Base() {
    cout << "Base()\n";
  }
  ~Base() {
    cout << "~Base()\n";;
  }
};

shared_ptr<Base> factory() {
  return make_shared<Base>(); // allocate on heap
}

void test() {
  shared_ptr<Base> p = factory();
  cout << "teset() will return\n";
}

class StrBlob {
private:
  shared_ptr<vector<string>> data;
public:
  StrBlob(): data(make_shared<vector<string>>()) {}
  StrBlob(initializer_list<string> il): data(make_shared<vector<string>>(il)) {}
  string &front() {
    return data->front();
  }
  string &back() {
    return data->back();
  }
  void push_back(const string &s) {
    data->push_back(s);
  }
  int size() {
    return data->size();
  }
};

void delete_int_p(int *p) {
  cout << "will delete p=" << *p << endl;
  delete p;
}

class MySharedPtr {
private:
  string *p;
public:
  MySharedPtr(string *pp): p{pp} {}
  string *get() {
    return p;
  }
  ~MySharedPtr() {
    cout << "will delete p" << endl;
    delete p;
  }
};



int main(int argc, char **argv) {
  shared_ptr<string> p1; // pointer to nullptr
  shared_ptr<list<int>> p2;
  shared_ptr<int> p3 = make_shared<int>(3);
  shared_ptr<string> p4 = make_shared<string>(10, 'X');
  shared_ptr<int> p5 = make_shared<int>();
  auto p6 = make_shared<string>();
  cout << p1 << endl;
  cout << p2 << endl;
  cout << p3 << "=" << *p3 << endl;
  cout << p4 << "=" << *p4 << endl;
  cout << p5 << "=" << *p5 << endl;
  cout << p6 << "=" << *p6 << endl;
  auto p = make_shared<int>(52);
  auto q(p);
  cout << p << "=" << *p << "|" << p.use_count() << endl;
  p.reset();
  cout << q << "=" << *q << "|" << q.use_count() << endl;
  test();
  cout << "test() return\n";
  StrBlob strBlob{"abc", "def", "ghi"};
  cout << strBlob.front() << "|" << strBlob.back() << endl;
  strBlob = {"a", "b", "c"};
  cout << strBlob.front() << "|" << strBlob.back() << endl;
  StrBlob b1;
  {
    StrBlob b2 = {"a", "an", "the"};
    b1 = b2;
    b2.push_back("about");
    cout << b1.size() << "|" << b2.size() << endl; // 4|4
  }
  cout << b1.size() << endl;
  //
  int *pint = new int(33);
  cout << *pint << endl;
  delete pint;
  pint == nullptr; // avoid dangling pointer
  shared_ptr<int> spint(new int(42)); // Not so good;
  shared_ptr<int> spingGood = make_shared<int>(42); //Good
  // BAD
  shared_ptr<int> badP(new int(24));
  int *pp = badP.get();
  {
    //shared_ptr<int> q(pp); // inner pointer is delted when deleting q
  }
  cout << *badP << endl; 
  // my deleter
  {
    shared_ptr<int> ppp(new int(32), delete_int_p); // my deleter
  }
  {
    string *str = new string("abc");
    MySharedPtr strShared(str);
    cout << *strShared.get() << endl;
  }
  cout << "EXIT" << endl;
  return 0;
}