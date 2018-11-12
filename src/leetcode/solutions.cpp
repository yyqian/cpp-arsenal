#include <vector>
#include <string>

using namespace std;

vector<string> letterCombinations(string digits) {
  vector<string> dict(10);
  vector<string> res;
  dfs(digits, 0, res, "", dict);
  return res;
}

void dfs(string &digits, int i, vector<string> &res, const string& path, vector<string> &dict) {
  if (i == digits.size()) {
    res.push_back(path);
    return;
  }
  char c = digits[i];
  string letters = dict[c - '0'];
  for (int j = 0; j < letters.size(); ++j) {
    dfs(digits, i + 1, res, path + letters[j], dict);
  }
}

// 2552551135
// in: 
void dfs(const string &in, int idx, vector<string> &path, vector<string> &res) {
  // 结束条件
  if (idx == 5) {
    if (in.empty()) {
      res.push_back(path[0] + "." + path[1] + "." + path[2] + "." + path[3]);
    }
    return;
  }
  // 遍历
  for (int i = 1; i <= 3; ++i) {
    if (i < in.size()) {
      string part = in.substr(0, i);
      if (atoi(part.c_str()) > 255) continue;
      path.push_back(part);
      dfs(in.substr(i), idx + 1, path, res);
      path.pop_back();
    }
  }
}

vector<string> restoreIpAddresses(string s) {
  vector<string> res;
  vector<string> path;
  dfs(s, 1, path, res);
  return res;
}

//
bool isValid(string &input, int i, int j) {
  while (i < j) {
    if (input[i++] != input[j--]) return false;
  }
  return true;
}

void dfs(string &input, int idx, vector<string> &path, vector<vector<string>> &res) {
  // 终止条件
  if (idx == input.size()) {
    res.push_back(path);
    return;
  }
  for (int i = idx + 1; i <= input.size(); ++i) {
    if (isValid(input, idx, i)) {
      path.push_back(input.substr(idx, i - idx));
      dfs(input, i, path, res);
      path.pop_back();
    }
  }
}

    vector<vector<string>> partition(string s) {
         vector<string> out;
         vector<vector<string>> res;
         dfs(s, 0, out, res);
         return res;
    }

// 46
void dfs(vector<int> &input, vector<int> &path, vector<vector<int>> &res, int idx, vector<bool> &visited) {
  if (idx == input.size()) {
    res.push_back(path);
    return;
  }
  for (int i = 0; i < input.size(); ++i) {
    if (!visited[i]) {
      visited[i] = true;
      path.push_back(input[i]);
      dfs(input, path, res, idx + 1, visited);
      path.pop_back();
      visited[i] = false;
    }
  }
}

// 47
void dfs(vector<int> &input, vector<int> &path, vector<vector<int>> &res, int idx, vector<bool> &visited) {
  if (idx == input.size()) {
    res.push_back(path);
    return;
  }
  for (int i = 0; i < input.size(); ++i) {
    if (!visited[i]) {
      if (i > 0 && input[i] == input[i - 1] && !visited[i - 1]) continue;
      visited[i] = true;
      path.push_back(input[i]);
      dfs(input, path, res, idx + 1, visited);
      path.pop_back();
      visited[i] = false;
    }
  }
}

// 77
void dfs(int n, int k, int start, int idx, vector<vector<int>> &res, vector<int> &path) {
  if (idx == k) {
    res.push_back(path);
    return;
  }
  for (int i = start; i <= n; ++i) {
    path.push_back(i);
    dfs(n, k, i + 1, idx + 1, res, path);
    path.pop_back();
  }
}

//39
void dfs(vector<int> &input, int target, int start, vector<int> &path, vector<vector<int>> &res) {
  if (target == 0) {
    res.push_back(path);
    return;
  }
  for (int i = start; i < input.size(); ++i) {
    if (target - input[i] >= 0) {
      path.push_back(input[i]);
      dfs(input, target - input[i], i, path, res);
      path.pop_back();
    }
  }
}

// 40
void dfs(vector<int> &input, int target, int start, vector<int> &path, vector<vector<int>> &res) {
  if (target == 0) {
    res.push_back(path);
    return;
  }
  for (int i = start; i < input.size(); ++i) {
    if (target - input[i] >= 0 && (i == start || input[i] != input[i - 1])) {
      path.push_back(input[i]);
      dfs(input, target - input[i], i, path, res);
      path.pop_back();
    }
  }
}

// 216
void dfs(int start, int target, int k, vector<int> &path, vector<vector<int>> &res) {
  if (path.size() == k) {
    if (target == 0) res.push_back(path);
    return;
  }
  for (int i = start; i <= 9; ++i) {
    if (target - i >= 0) {
      path.push_back(i);
      dfs(i + 1, target - i, k, path, res);
      path.pop_back();
    }
  }
}

// 78
void dfs(vector<int> &input, int start, vector<int> &path, vector<vector<int>> &res) {
  if (start == input.size()) {
    res.push_back(path);
    return;
  }
  dfs(input, start + 1, path, res);
  path.push_back(input[start]);
  dfs(input, start, path, res);
  path.pop_back();
}

// 90

