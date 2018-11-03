#include <condition_variable>
#include <mutex>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <string>

using namespace std;

class ReadWriteLock {
public:
  ReadWriteLock() : readers(0), has_writer(false) {}
  void read_lock() {
    unique_lock<mutex> lk(m);
    while (has_writer) {
      no_writer.wait(lk);
    }
    ++readers;
    ostringstream oss;
    oss << "reader " << this_thread::get_id() << " acquired lock\n";
    cout << oss.str();
  }
  void read_unlock() {
    lock_guard<mutex> lk(m);
    --readers;
    if (readers == 0) no_reader.notify_one();
    ostringstream oss;
    oss << "reader " << this_thread::get_id() << " will release lock\n";
    cout << oss.str();
  }
  void write_lock() {
    unique_lock<mutex> lk(m);
    while (readers != 0 || has_writer) {
      no_reader.wait(lk);
    }
    has_writer = true;
    cout << "writer acquired lock\n";
  }
  void write_unlock() {
    lock_guard<mutex> lk(m);
    has_writer = false;
    no_writer.notify_all();
    cout << "writer will release lock\n";
  }
private:
  int readers;
  bool has_writer;
  condition_variable no_reader;
  condition_variable no_writer;
  mutex m;
};


int main(int argc, char **argv) {
  ReadWriteLock readWriteLock;
  for (int i = 0; i < 3; ++i) {
    thread t([&readWriteLock] {
      readWriteLock.read_lock();
      this_thread::sleep_for(chrono::seconds(1));
      readWriteLock.read_unlock();
    });
    t.detach();
  }
  thread t([&readWriteLock] {
    readWriteLock.write_lock();
    this_thread::sleep_for(chrono::seconds(1));
    readWriteLock.write_unlock();
  });
  t.detach();
  this_thread::sleep_for(chrono::seconds(30));
  return 0;
}