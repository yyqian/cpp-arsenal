#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

class Counter {
private:
  int val;
  pthread_mutex_t lock;
public:
  Counter(): val{0} {
    pthread_mutex_init(&lock, NULL);
  }
  void inc() {
    pthread_mutex_lock(&lock);
    ++val;
    pthread_mutex_unlock(&lock);
  }
  void decr() {
    pthread_mutex_lock(&lock);
    --val;
    pthread_mutex_unlock(&lock);
  }
  int get() {
    pthread_mutex_lock(&lock);
    int res = val;
    pthread_mutex_unlock(&lock);
    return res;
  }
};

struct ListNode {
  int val;
  ListNode* next;
  ListNode(int x): val{x}, next{nullptr} {}
};

class List {
private:
  ListNode* dummy;
  int n;
  pthread_mutex_t lock;
public:
  List(): dummy{new ListNode(0)}, n{0}, lock(PTHREAD_MUTEX_INITIALIZER) {}
  int insert(int v) {
    ListNode *head = new ListNode(v);
    pthread_mutex_lock(&lock);
    head->next = dummy->next;
    dummy->next = head;
    ++n;
    pthread_mutex_unlock(&lock);
    return 0;
  }
  int find(int v) {
    pthread_mutex_lock(&lock);
    ListNode *curr = dummy->next;
    int i = 0;
    while (curr) {
      if (curr->val == v) {
        pthread_mutex_unlock(&lock);
        return i;
      }
      ++i;
      curr = curr->next;
    }
    pthread_mutex_unlock(&lock);
    return -1;
  }
  void print() {
    pthread_mutex_lock(&lock);
    ListNode *curr = dummy->next;
    cout << "list: ";
    while (curr) {
      cout << curr->val << " ";
      curr = curr->next;
    }
    pthread_mutex_unlock(&lock);
    cout << endl;
  }
  int size() {
    return n;
  }
};

class Queue {
private:
  ListNode* dummy;
  ListNode* tail;
  int n;
  pthread_mutex_t lock;
public:
  Queue(): n{0}, lock(PTHREAD_MUTEX_INITIALIZER) {
    dummy = new ListNode(0);
    tail = dummy;
  }
  void push(int v) {
    ListNode *curr = new ListNode(v);
    pthread_mutex_lock(&lock);
    tail->next = curr;
    tail = curr;
    ++n;
    pthread_mutex_unlock(&lock);
  }
  void pop() {
    pthread_mutex_lock(&lock);
    ListNode *oldHead = dummy->next;
    if (oldHead) {
      dummy->next = oldHead->next;
    }
    --n;
    pthread_mutex_unlock(&lock);
    delete oldHead;
  }
  int front() {
    pthread_mutex_lock(&lock);
    int res = dummy->next ? dummy->next->val : -1;
    pthread_mutex_unlock(&lock);
    return res;
  }
  void print() {
    pthread_mutex_lock(&lock);
    ListNode *curr = dummy->next;
    cout << "queue: ";
    while (curr) {
      cout << curr->val << " ";
      curr = curr->next;
    }
    pthread_mutex_unlock(&lock);
    cout << endl;
  }
};

class HashTable {
private:
  int sz;
  vector<List *> buckets;
public:
  HashTable(int n): sz{n}, buckets(n, new List()) {}
  void put(int key) {
    int bucket = key % sz;
    buckets[bucket]->insert(key);
  }
  int get(int key) {
    int bucket = key % sz;
    return buckets[bucket]->find(key);
  }
};

void *counter_test(void *arg) {
  Counter *counter = (Counter *)arg;
  for (int i = 0; i < 100000; ++i) {
    counter->inc();
    counter->decr();
    counter->inc();
  }
  return nullptr;
}

void *list_test(void *arg) {
  List *list = (List *)arg;
  for (int i = 0; i < 100; ++i) {
    list->insert(i);
  }
  return nullptr;
}

void *queue_test(void *arg) {
  Queue *q = (Queue *)arg;
  for (int i = 0; i < 100; ++i) {
    q->push(i);
  }
}

#define THREAD_N 4

int main() {
  pthread_t p[THREAD_N];
  // Counter Test
  Counter counter;
  for (int i = 0; i < THREAD_N; ++i) {
    pthread_create(p + i, NULL, counter_test, &counter);
  }
  for (int i = 0; i < THREAD_N; ++i) {
    pthread_join(p[i], NULL);
  }
  cout << "counter.get() = " << counter.get() << endl;
  // List Test
  List lst;
  for (int i = 0; i < THREAD_N; ++i) {
    pthread_create(p + i, NULL, list_test, &lst);
  }
  for (int i = 0; i < THREAD_N; ++i) {
    pthread_join(p[i], NULL);
  }
  cout << "lst[" << lst.find(97) << "] = 97, " << "lst.size() = " << lst.size() << endl;
  lst.print();
  // Queue Test
  Queue q;
  for (int i = 0; i < THREAD_N; ++i) {
    pthread_create(p + i, NULL, queue_test, &q);
  }
  for (int i = 0; i < THREAD_N; ++i) {
    pthread_join(p[i], NULL);
  }
  q.print();
  return EXIT_SUCCESS; //cstdlib
}
