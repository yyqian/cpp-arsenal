#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;

class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack<int> stk;
        for (string token : tokens) {
            if (token == "+" || token == "-" || token == "*" || token == "/") {
                int r = stk.top();
                stk.pop();
                int l = stk.top();
                stk.pop();
                cout << l << token << r << endl;
                if (token == "+") {
                    stk.push(l + r);
                } else if (token == "-") {
                    stk.push(l - r);
                } else if (token == "*") {
                    stk.push(l * r);
                } else if (token == "/") {
                    stk.push(l / r);
                }
            } else {
                stk.push(toInt(token));
            }
        }
        return stk.top();
    }
    int toInt(string &str) {
        int sign = str[0] == '-' ? -1 : 1;
        int res = 0;
        for (char c : str) {
            if (c == '-') continue;
            res = res * 10 + (c - '0');
        }
        return sign * res;
    }
};

int main() {
  Solution sol;
  vector<string> nums{"10","6","9","3","+","-11","*","/","*","17","+","5","+"};
  int res = sol.evalRPN(nums);
  return 0;
}