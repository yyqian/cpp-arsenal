#include <iostream>
#include <string>
#include <vector>
using namespace std;

int *aux = nullptr;

void merge(vector<int> &nums, int lo, int mid, int hi) {
  int i = lo;
  int j = mid + 1;
  for (int k = lo; k <= hi; ++k) {
    if (i > mid) {
      aux[k] = nums[j++];
    } else if (j > hi) {
      aux[k] = nums[i++];
    } else if (nums[i] < nums[j]) {
      aux[k] = nums[i++];
    } else {
      aux[k] = nums[j++];
    }
  }
  for (int k = lo; k <= hi; ++k) {
    nums[k] = aux[k];
  }
}

void mergesort(vector<int> &nums, int lo, int hi) {
  if (lo >= hi) {
    return;
  }
  int mid = lo + (hi - lo) / 2;
  mergesort(nums, lo, mid);      // 排序左半边
  mergesort(nums, mid + 1, hi);  // 排序右半边
  merge(nums, lo, mid, hi);      // 合并两边
}

int main() {
  vector<int> nums{1, 2, 4, 5, 32, 42, 123, 43, 45};
  aux = new int[nums.size()];  // 空间复杂度为 N
  mergesort(nums, 0, nums.size() - 1);
  delete[] aux;
  for (int i : nums) {
    cout << to_string(i) << " ";
  }
  cout << endl;
  return 0;
}