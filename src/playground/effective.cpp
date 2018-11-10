class Empty {};

void DefaultFunc() {
  Empty e1;      // default 构造函数
  Empty e2(e1);  // copy 构造函数
  e2 = e1;       // copy assignment 操作符
}
