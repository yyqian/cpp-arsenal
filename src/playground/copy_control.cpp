#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

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
  HasPtrPointLike(const std::string &s = std::string()) : ps(new std::string(s)), i(0), use(new std::size_t(1)) {}
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

class StrVec {
public:
  StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr) {}
  StrVec(const StrVec&);
  ~StrVec();
  StrVec(StrVec &&s) noexcept
    : elements(s.elements), first_free(s.first_free), cap(s.cap) {
    s.elements = nullptr;
    s.first_free = nullptr;
    s.cap = nullptr;
  }
  StrVec &operator=(StrVec &&rhs) noexcept {
    if (this != &rhs) {
      free();
      elements = rhs.elements;
      first_free = rhs.first_free;
      cap = rhs.cap;
      rhs.elements = nullptr;
      rhs.first_free = nullptr;
      rhs.cap = nullptr;
    }
    return *this;
  }
  void push_back(const std::string&);
  void push_back(std::string &&s) {
    chk_n_alloc();
    alloc.construct(first_free++, std::move(s));
  }
  size_t size() const {
    return first_free - elements;
  }
  size_t capacity() const {
    return cap - elements;
  }
  std::string *begin() const {
    return elements;
  }
  std::string *end() const {
    return first_free;
  }
private:
  std::allocator<std::string> alloc;
  std::string *elements;
  std::string *first_free;
  std::string *cap;
  void free();
  void reallocate();
  std::pair<std::string*, std::string*> alloc_n_copy(const std::string*, const std::string*);
  void chk_n_alloc() {
    if (size() == capacity()) {
      reallocate();
    }
  }
};

void StrVec::push_back(const std::string &s) {
  chk_n_alloc();
  alloc.construct(first_free++, s);
}

std::pair<std::string*, std::string*> StrVec::alloc_n_copy(const std::string *b, const std::string *e) {
  auto data = alloc.allocate(e - b);
  return {data, std::uninitialized_copy(b, e, data)};
}

void StrVec::free() {
  if (elements) {
    for (auto p = first_free; p != elements;) {
      alloc.destroy(--p);
    }
    alloc.deallocate(elements, cap - elements);
  }
}

StrVec::StrVec(const StrVec &s) {
  auto newdata = alloc_n_copy(s.begin(), s.end());
  elements = newdata.first;
  first_free = newdata.second;
  cap = newdata.second;
}

StrVec::~StrVec() {
  free();
}

void StrVec::reallocate() {
  auto newcapacity = size() ? 2 * size() : 1;
  auto newdata = alloc.allocate(newcapacity);
  auto dest = newdata;
  auto elem = elements;
  for (size_t i = 0; i != size(); ++i) {
    alloc.construct(dest++, std::move(*elem++));
  }
  free();
  elements = newdata;
  first_free = dest;
  cap = elements + newcapacity;
}

void rvalue_demo() {
  int i = 42;
  int &ri = i;
  const int &r2 = i * 42;
  int &&rr2 = i * 42;
  int &&rr3 = std::move(i);
}

struct X {
  int i;
  std::string s;
};

struct hasX {
  X mem;
};


void rvalue_test() {
  X x;
  X x2 = std::move(x);
  hasX hx;
  hasX hx2 = std::move(hx);
}

class Foo {
public:
  Foo sorted() && {
    std::sort(data.begin(), data.end());
    return *this;
  };
  Foo sorted() const & {
    Foo ret(*this);
    std::sort(ret.data.begin(), ret.data.end());
    return ret;
  }
private:
  std::vector<int> data;
};
