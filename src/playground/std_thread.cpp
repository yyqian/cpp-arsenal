#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <memory>
#include <mutex>
#include <condition_variable>

using namespace std;

void hello(const char *s) {
  cout << this_thread::get_id() << "|Hello, " << s << endl;
}

mutex mtx;

void myinc(shared_ptr<int> cnt) {
  for (int i = 0; i < 1000000; ++i) {
    lock_guard<mutex> lock(mtx); // RAII style lock
    //mtx.lock();
    ++(*cnt);
    //mtx.unlock();
  }
}

int main(int argc, char **argv) {
  thread t0(hello, "world");
  cout << t0.get_id() << endl;
  cout << t0.joinable() << endl;
  t0.detach();
  cout << t0.joinable() << endl;
  this_thread::sleep_for(chrono::seconds(1));
  shared_ptr<int> cnt = make_shared<int>(0);
  thread t1(myinc, cnt);
  thread t2(myinc, cnt);
  t1.join();
  t2.join();
  cout << *cnt << endl;
  return 0;
}