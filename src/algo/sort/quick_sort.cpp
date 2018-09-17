#include <iostream>
#include <string>
#include <vector>
using namespace std;

void swap(vector<int> &nums, int i, int j) {
  int tmp = nums[i];
  nums[i] = nums[j];
  nums[j] = tmp;
}

// 这块要好好看下
int partition(vector<int> &nums, int lo, int hi) {
  int pivot = nums[lo];
  int i = lo;
  int j = hi + 1;
  while (true) {
    while (nums[++i] < pivot) {
      if (i == hi) break;
    }
    while (nums[--j] > pivot) {
      if (j == lo + 1) break;
    }
    if (i >= j) break;
    swap(nums, i, j);
  }
  swap(nums, lo, j);
  return j;
}

void quicksort(vector<int> &nums, int lo, int hi) {
  if (lo >= hi) {
    return;
  }
  int pivot = partition(nums, lo, hi);
  quicksort(nums, lo, pivot - 1);
  quicksort(nums, pivot + 1, hi);
}

int main() {
  vector<int> nums{1, 2, 4, 5, 32, 42, 123, 43, 45};
  quicksort(nums, 0, nums.size() - 1);
  for (int i : nums) {
    cout << to_string(i) << " ";
  }
  cout << endl;
  return 0;
}