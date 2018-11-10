#include <memory>
#include <string>
#include <vector>

template <typename T>
inline int compare(const T &v1, const T &v2) {
  if (v1 < v2) return -1;
  if (v2 < v1) return 1;
  return 0;
}

template <typename T>
class Blob {
 public:
  typedef T value_type;
  typedef typename std::vector<T>::size_type size_type;
  Blob();
  Blob(std::initializer_list<T> il);
  size_type size() const { return data->size(); }
  bool empty() const { return data->empty(); }
  void push_back(T &&t) { data->push_back(t); }
  void pop_back();
  T &back();
  T &operator[](size_type i);

 private:
  std::shared_ptr<std::vector<T>> data;
  void check(size_type i, const std::string &msg) const;
};