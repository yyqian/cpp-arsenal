#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

mutex mtx;
condition_variable notFull;
condition_variable notEmpty;
const int Q_SIZE = 10;

void consumer(shared_ptr<queue<int>> queuePtr) {
  while (true) {
    unique_lock<mutex> lk(mtx);
    while (queuePtr->empty()) {
      cout << "consumer sleep\n";
      notEmpty.wait(lk);
    }
    int ele = queuePtr->front();
    queuePtr->pop();
    lk.unlock();
    notFull.notify_one();
    cout << "consume: " + to_string(ele) + "\n";
  }
}

void producer(shared_ptr<queue<int>> queuePtr) {
  for (int i = 0; i < 100; ++i) {
    unique_lock<mutex> lk(mtx);
    notFull.wait(lk, [&queuePtr]() {return queuePtr->size() < Q_SIZE;});
    queuePtr->push(i);
    lk.unlock();
    notEmpty.notify_one();
    cout << "produce: " + to_string(i) + "\n";
  }
}

template <typename T>
class Buffer {
public:
  Buffer(int cap) : data(cap), n{0}, fill_ptr{0}, use_ptr{0} {}
  T take() {
    unique_lock<mutex> lk(m);
    while (n == 0) {
      cout << "consumer sleep\n";
      not_empty.wait(lk);
    }
    T ret = data[use_ptr];
    use_ptr = (use_ptr + 1) % data.size();
    --n;
    lk.unlock();
    not_full.notify_all();
    return ret;
  }
  void put(T x) {
    unique_lock<mutex> lk(m);
    while (n == data.size()) {
      cout << "producer sleep\n";
      not_full.wait(lk);
    }
    data[fill_ptr] = x;
    fill_ptr = (fill_ptr + 1) % data.size();
    ++n;
    lk.unlock();
    not_empty.notify_all();
  }
private:
  mutex m;
  condition_variable not_full;
  condition_variable not_empty;
  vector<T> data;
  int fill_ptr;
  int use_ptr;
  int n;
};

void test0() {
  int consumerCnt = 1;
  shared_ptr<queue<int>> queuePtr = make_shared<queue<int>>();
  vector<shared_ptr<thread>> consumerVec;
  for (int i = 0; i < consumerCnt; ++i) {
    consumerVec.push_back(make_shared<thread>(consumer, queuePtr));
  }
  for (int i = 0; i < 3; ++i) {
    thread t(producer, queuePtr);
    t.detach();
  }
  for (int i = 0; i < consumerCnt; ++i) {
    consumerVec[i]->join();
  }
}

void test1() {
  Buffer<int> buf(10);
  thread con([&] {
    while (true) {
      cout << "consume: " + to_string(buf.take()) + "\n";
    }
  });
  thread pro([&] {
    for (int i = 0; i < 40; ++i) {
      this_thread::sleep_for(chrono::milliseconds(1));
      buf.put(i);
    }
  });
  con.join();
}

int main(int argc, char **argv) {
  test0();
  return 0;
}
