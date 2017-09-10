#include <iostream>
#include <string>

struct NoCopy {
  NoCopy() = default; // default constructor
  NoCopy(const NoCopy&) = delete; // copy constructor
  NoCopy& operator=(const NoCopy&) = delete; // copy-assignment operator
  ~NoCopy() = default; // destructor
};

class HasPtr {
  friend void swap(HasPtr&, HasPtr&);
public:
  HasPtr(const std::string &s = std::string()) : ps(new std::string(s)), i(0) {}
  HasPtr(const HasPtr &p) : ps(new std::string(*(p.ps))), i(p.i) {};
  HasPtr& operator=(const HasPtr &);
  ~HasPtr() { delete ps; }
private:
  std::string *ps;
  int i;
};

HasPtr& HasPtr::operator=(const HasPtr &rhs) {
  auto newp = new std::string(*(rhs.ps));
  delete ps;
  ps = newp;
  i = rhs.i;
  return *this;
}

class HasPtrPointLike {
public:
  HasPtrPointLike(const std::string &s = std::string()): ps(new std::string(s)), i(0), use(new std::size_t(1)) {}
  HasPtrPointLike(const HasPtrPointLike &p) : ps(p.ps), i(p.i), use(p.use) { ++*use; }
  HasPtrPointLike& operator=(const HasPtrPointLike&);
  ~HasPtrPointLike();
private:
  std::string *ps;
  int i;
  std::size_t *use;
};

HasPtrPointLike& HasPtrPointLike::operator=(const HasPtrPointLike &rhs) {
  ++*rhs.use;
  if (--*use == 0) {
    delete ps;
    delete use;
  }
  ps = rhs.ps;
  i = rhs.i;
  use = rhs.use;
  return *this;
}

HasPtrPointLike::~HasPtrPointLike() {
  if (--(*use) == 0) {
    delete ps;
    delete use;
  }
}

inline void swap(HasPtr &lhs, HasPtr &rhs) {
  using std::swap;
  swap(lhs.ps, rhs.ps);
  swap(lhs.i, rhs.i);
}
