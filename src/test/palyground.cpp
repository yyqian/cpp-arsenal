#include "gtest/gtest.h"

using namespace std;

class Element {
public:
  Element()
    :val(0) {
    cout << "Element default constructed" << endl;
  }
  ~Element() {
    cout << "Element will be destructed, current val: " << to_string(val) << endl;
  }
  int getVal() {
    return val;
  }
  Element &setVal(int v) {
    val = v;
    return *this;
  }
private:
  int val;
};

class Combo {
public:
  Combo()
    :ele() {
    cout << "Combo default constructed" << endl;
  }
  ~Combo() {
    cout << "Combo will be destructed, current ele.val: " << to_string(ele.getVal()) << endl;
  }
  Element &getEle() {
    return ele;
  }
private:
  Element ele;
};

TEST(class_design, construct) {
  Combo *combo = new Combo();
  combo->getEle().setVal(99).setVal(88);
  delete combo;
}