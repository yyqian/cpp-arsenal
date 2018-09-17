#include <vector>
using namespace std;
class MaxHeap {
 public:
  MaxHeap(int sz) : data(sz + 1), N(sz) {}
  void insert(int x) {
    data.push_back(x);
    N++;
    swim(data.size() - 1);
  }
  int del() {
    int res = data[1];
    swap(1, N);
    data.pop_back();
    N--;
    sink(1);
    return res;
  }

 private:
  vector<int> data;
  int N;
  void swim(int i) {
    while (i > 1) {
      int j = i / 2;
      if (j > 0 && data[j] >= data[i]) {
        break;
      }
      i = j;
    }
  }
  void sink(int i) {
    while (i <= N / 2) {
      int j = 2 * i;
      if (j < N && data[j] < data[j + 1]) {
        j++;
      }
      if (data[i] >= data[j]) {
        break;
      }
      swap(i, j);
      i = j;
    }
  }
  void swap(int i, int j) {
    int tmp = data[i];
    data[i] = data[j];
    data[j] = tmp;
  }
};