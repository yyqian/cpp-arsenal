#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

// Data Struct

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x): val(x), next(nullptr) {}
};

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
};

// LEVEL-ORDER

vector<vector<int>> level_order(TreeNode *root) {
  vector<vector<int>> res;
  queue<TreeNode *> q;
  q.push(root);
  while (!q.empty()) {
    int sz = q.size();
    vector<int> level;
    for (int i = 0; i < sz; ++i) {
      TreeNode *p = q.front();
      q.pop();
      level.push_back(p->val);
      if (p->left) q.push(p->left);
      if (p->right) q.push(p->right);
    }
    res.push_back(level);
  }
  return res;
}

// IN-ORDER

void inorder_helper(TreeNode *root, vector<int> &res) {
  if (!root) return;
  inorder_helper(root->left, res);
  res.push_back(root->val);
  inorder_helper(root->right, res);
}

vector<int> inorder_recur(TreeNode *root) {
  vector<int> res;
  inorder_helper(root, res);
  return res;
}

vector<int> inorder_iter(TreeNode *root) {
  vector<int> res;
  stack<TreeNode *> stk;
  TreeNode *p = root;
  while (p) {
    stk.push(p);
    p = p->left;
  }
  while (!stk.empty()) {
    p = stk.top();
    stk.pop();
    res.push_back(p->val);
    p = p->right;
    while (p) {
      stk.push(p);
      p = p->left;
    }
  }
  return res;
}

// PRE-ORDER

vector<int> preorder_iter(TreeNode *root) {
  vector<int> res;
  stack<TreeNode *> stk;
  TreeNode *p = root;
  while (p) {
    res.push_back(p->val);
    stk.push(p);
    p = p->left;
  }
  while (!stk.empty()) {
    p = stk.top();
    stk.pop();
    p = p->right;
    while (p) {
      res.push_back(p->val);
      stk.push(p);
      p = p->left;
    }
  }
  return res;
}

// TOPOLOGICAL SORT

void topo_dfs(vector<vector<int>> &graph, int node, vector<bool> &visited, vector<int> &res) {
  if (visited[node]) return;
  visited[node] = true;
  for (int i : graph[node]) {
    topo_dfs(graph, i, visited, res);
  }
  res.push_back(node);
}

vector<int> topo_sort(vector<vector<int>> &graph) {
  vector<int> res;
  vector<bool> visited(graph.size(), false);
  for (int i = 0; i < graph.size(); ++i) {
    topo_dfs(graph, i, visited, res);
  }
  reverse(res.begin(), res.end());
  return res;
}

// Quick Sort
int partition(vector<int> &v, int lo, int hi) {
  int target = v[lo];
  int i = lo;
  int j = hi + 1;
  while (true) {
    while (v[++i] < target) {
      if (i == hi) break;
    }
    while (v[--j] > target) {
      if (j == lo + 1) break;
    }
    if (i >= j) break;
    swap(v[i], v[j]);
  }
  swap(v[lo], v[j]);
  return j;
}

void quick_sort(vector<int> &v, int lo, int hi) {
  if (lo >= hi) return;
  int p = partition(v, lo, hi);
  quick_sort(v, lo, p - 1);
  quick_sort(v, p + 1, hi);
}

// Merge Sort: [lo, mid], [mid + 1, hi]
void merge(vector<int> &vec, vector<int> &aux, int lo, int mid, int hi) {
  int i = lo;
  int j = mid + 1;
  for (int k = lo; k <= hi; ++k) {
    if (j == hi + 1 || (i <= mid && vec[i] < vec[j])) {
      aux[k] = vec[i++];
    } else {
      aux[k] = vec[j++];
    }
  }
  for (int k = lo; k <= hi; ++k) {
    vec[k] = aux[k];
  }
}

void merge_sort_helper(vector<int> &vec, vector<int> &aux, int lo, int hi) {
  if (lo >= hi) return;
  int mid = lo + (hi - lo) / 2;
  merge_sort_helper(vec, aux, lo, mid);
  merge_sort_helper(vec, aux, mid + 1, hi);
  merge(vec, aux, lo, mid, hi);
}

void merge_sort(vector<int> &vec) {
  vector<int> aux(vec.size());
  merge_sort_helper(vec, aux, 0, vec.size() - 1);
}

// MinHeap
class MinHeap {
private:
  vector<int> data;
  int size;
  void sink(int i) {
    while (i <= size / 2) {
      int j = i * 2;
      if (j < size && data[j + 1] < data[j]) ++j;
      if (data[i] > data[j]) {
        swap(data[i], data[j]);
        i = j;
      } else {
        break;
      }
    }
  }
  void swim(int i) {
    while (i > 1) {
      int j = i / 2;
      if (data[j] > data[i]) {
        swap(data[i], data[j]);
        i = j;
      } else {
        break;
      }
    }
  }
public:
  MinHeap(): data(1), size{0} {}
  void pop() {
    data[1] = data[size];
    data.pop_back();
    --size;
    sink(1);
  }
  int top() {
    return data[1];
  }
  void push(int n) {
    data.push_back(n);
    ++size;
    swim(size);
  }
};

// Trie
struct TrieNode {
  bool isWord;
  vector<TrieNode *> children;
  TrieNode(bool word): isWord{word}, children(255) {}
};

class Trie {
private:
  TrieNode *root;
public:
  Trie(): root(new TrieNode(false)) {}
  void add(string &s) {
    TrieNode *p = root;
    for (int i = 0; i < s.size(); ++i) {
      char c = s[i];
      if (!p->children[c]) {
        p->children[c] = new TrieNode(false);
      }
      p = p->children[c];
    }
    p->isWord = true;
  }
  bool exist(string &s) {
    TrieNode *p = find(s);
    return p != nullptr && p->isWord;
  }
  TrieNode *find(string &s) {
    TrieNode *p = root;
    for (char c : s) {
      p = p->children[c];
      if (!p) return nullptr;
    }
    return p;
  }
};

// linkedlist
ListNode *reverse_list(ListNode *head) {
  ListNode *dummy = new ListNode(0);
  while (head) {
    ListNode *tmp = head->next;
    head->next = dummy->next;
    dummy->next = head;
    head = tmp;
  }
  return dummy->next;
}

ListNode *reverse_list_recur_helper(ListNode *head, ListNode *tail) {
  if (!head) {
    return tail;
  }
  ListNode *nxt = head->next;
  head->next = tail;
  return reverse_list_recur_helper(nxt, head);
}

ListNode *reverse_list_recur(ListNode *head) {
  if (!head) return nullptr;
  return reverse_list_recur_helper(head, nullptr);
}

// binary search: 4 5 6 7 0 1 2 3
int binary_search(vector<int> &nums, int val) {
  int lo = 0;
  int hi = nums.size() - 1;
  while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;
    if (nums[mid] == val) {
      return mid;
    } else if (nums[lo] < nums[mid]) {
      if (nums[lo] <= val && nums[mid] > val) {
        hi = mid - 1;
      } else {
        lo = mid + 1;
      }
    } else {
      if (nums[mid] < val && val <= nums[hi]) {
        lo = mid + 1;
      } else {
        hi = mid - 1;
      }
    }
  }
  return -1;
}

// backtracking
// DP
// two pointers
// shell sort

//----------------------Testing------------------------
//----------------------Testing------------------------
//----------------------Testing------------------------

/*
       4
   2        6
1    3    5   7
*/
TreeNode *prepareTree() {
  TreeNode *n00 = new TreeNode();
  n00->val = 4;
  TreeNode *n10 = new TreeNode();
  n10->val = 2;
  TreeNode *n11 = new TreeNode();
  n11->val = 6;
  TreeNode *n20 = new TreeNode();
  n20->val = 1;
  TreeNode *n21 = new TreeNode();
  n21->val = 3;
  TreeNode *n22 = new TreeNode();
  n22->val = 5;
  TreeNode *n23 = new TreeNode();
  n23->val = 7;
  n00->left = n10;
  n00->right = n11;
  n10->left = n20;
  n10->right = n21;
  n11->left = n22;
  n11->right = n23;
  return n00;
}

vector<vector<int>> prepareGraph() {
  vector<vector<int>> graph(6);
  graph[0] = {1};
  graph[1] = {2, 3};
  graph[2] = {3};
  graph[3] = {4};
  graph[5] = {2};
  return graph;
}

vector<int> prepareVector() {
  return {4, 32, 6, 46, 4, 32, 6, 0, -1};
}

ListNode *prepareLinkedList() {
  ListNode *nxt = new ListNode(1);
  ListNode *curr = new ListNode(2);
  curr->next = nxt;
  nxt = curr;
  curr = new ListNode(3);
  curr->next = nxt;
  nxt = curr;
  curr = new ListNode(4);
  curr->next = nxt;
  nxt = curr;
  curr = new ListNode(5);
  curr->next = nxt;
  return curr;
}

void printV(vector<int> &v) {
    for (int i : v) {
      cout << i << " ";
    }
    cout << endl;
}

int main() {
  TreeNode *root = prepareTree();
  // Tree Order
  vector<vector<int>> lvlOrder = level_order(root);
  for (vector<int> v : lvlOrder) {
    printV(v);
  }
  vector<int> inorder = inorder_recur(root);
  printV(inorder);
  inorder = inorder_iter(root);
  printV(inorder);
  vector<int> preorder = preorder_iter(root);
  printV(preorder);
  // Sort
  vector<vector<int>> graph = prepareGraph();
  vector<int> topo = topo_sort(graph);
  printV(topo);
  vector<int> v = prepareVector();
  quick_sort(v, 0, v.size() - 1);
  printV(v);
  v = prepareVector();
  merge_sort(v);
  printV(v);
  // MinHeap
  MinHeap pq;
  pq.push(3);
  pq.push(-1);
  pq.push(5);
  pq.push(23);
  pq.push(2);
  for (int i = 0; i < 5; ++i) {
    cout << pq.top() << " ";
    pq.pop();
  }
  cout << endl;
  // Trie
  string s0 = "abc";
  string s1 = "abcd";
  string s2 = "a";
  string s3 = "def";
  string s4 = "ab";
  Trie trie;
  trie.add(s0);
  trie.add(s1);
  trie.add(s2);
  cout << trie.exist(s0) << endl;
  cout << trie.exist(s1) << endl;
  cout << trie.exist(s2) << endl;
  cout << trie.exist(s3) << endl;
  cout << trie.exist(s4) << endl;
  // LinkedList
  ListNode *head = prepareLinkedList();
  head = reverse_list(head);
  while (head) {
    cout << head->val << " ";
    head = head->next;
  }
  cout << endl;
  head = prepareLinkedList();
  head = reverse_list_recur(head);
  while (head) {
    cout << head->val << " ";
    head = head->next;
  }
  cout << endl;
  // binary_search
  vector<int> nums{4,5,6,7,0,1,2,3};
  cout << binary_search(nums, 5) << endl;
  cout << binary_search(nums, 0) << endl;
  cout << binary_search(nums, 3) << endl;
  cout << binary_search(nums, 11) << endl;
  return 0;
}
