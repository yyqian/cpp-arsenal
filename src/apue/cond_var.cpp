#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

using namespace std;

bool ready = false;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *p0(void *arg) {
  pthread_mutex_lock(&lock);
  while (!ready) {
    printf("WAIT\n");
    pthread_cond_wait(&cond, &lock);
  }
  printf("READY\n");
  pthread_mutex_unlock(&lock);
  return NULL;
}

void *p1(void *arg) {
  pthread_mutex_lock(&lock);
  ready = true;
  printf("SIG\n");
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&lock);
  return NULL;
}

bool done = false;

void *child(void *arg) {
  pthread_mutex_lock(&lock);
  cout << "child get lock" << endl;
  done = true;
  pthread_cond_signal(&cond);
  cout << "child signaled" << endl;
  pthread_mutex_unlock(&lock);
}

// produce/consumer
pthread_cond_t empty_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t full_cond = PTHREAD_COND_INITIALIZER;
int q_count = 0;
int buffer = 0;

void *producer(void *arg) {
  for (int i = 0; i < 100; ++i) {
    pthread_mutex_lock(&lock);
    while (q_count == 1) {
      cout << "producer wait" << endl;
      pthread_cond_wait(&empty_cond, &lock);
      cout << "producer wakeup" << endl;
    }
    buffer = i;
    q_count = 1;
    cout << "data produced:" << buffer << endl;
    pthread_cond_signal(&full_cond);
    pthread_mutex_unlock(&lock);
  }
}

void *consumer(void *arg) {
  while (true) {
    pthread_mutex_lock(&lock);
    while (q_count == 0) {
      cout << "consumer wait" << endl;
      pthread_cond_wait(&full_cond, &lock);
      cout << "consumer wakeup" << endl;
    }
    cout << "data consumed:" << buffer << endl;
    q_count = 0;
    pthread_cond_signal(&empty_cond);
    pthread_mutex_unlock(&lock);
  }
}

// producer/consumer
class Buffer {
private:
  pthread_cond_t not_empt;
  pthread_cond_t not_full;
  pthread_mutex_t lock;
  int n;
  int size;
  int fill_ptr;
  int use_ptr;
  int *data;
public:
  Buffer(int sz): size(sz), n(0), fill_ptr(0), use_ptr(0), not_empt(PTHREAD_COND_INITIALIZER), not_full(PTHREAD_COND_INITIALIZER), lock(PTHREAD_MUTEX_INITIALIZER) {
    data = (int *)malloc(sz * sizeof(int));
  }
  ~Buffer() {
    pthread_cond_destroy(&not_empt);
    pthread_cond_destroy(&not_full);
    pthread_mutex_destroy(&lock);
    free(data);
  }
  void produce(int x) {
    pthread_mutex_lock(&lock);
    while (n == size) {
      cout << "producer sleep, n=" << n << endl;
      pthread_cond_wait(&not_full, &lock);
      cout << "producer wakeup, n=" << n << endl;
    }
    data[fill_ptr] = x;
    fill_ptr = (fill_ptr + 1) % size;
    ++n;
    cout << "producer signal, n=" << n << endl;
    pthread_cond_signal(&not_empt);
    pthread_mutex_unlock(&lock);
  }
  int consume() {
    pthread_mutex_lock(&lock);
    while (n == 0) {
      cout << "consumer sleep, n=" << n << endl;
      pthread_cond_wait(&not_empt,  &lock);
      cout << "consumer wakeup, n=" << n << endl;
    }
    int res = data[use_ptr];
    use_ptr = (use_ptr + 1) % size;
    --n;
    cout << "consumer signal, n=" << n << endl;
    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&lock);
    return res;
  }
};

void *buffer_consume(void *arg) {
  Buffer *buffer = (Buffer *)arg;
  while (true) {
    int stuff = buffer->consume();
    cout << "consumed " << stuff << endl;
  }
  return nullptr;
}

void *buffer_produce(void *arg) {
  Buffer *buffer = (Buffer *)arg;
  for (int i = 0; i < 50; ++i) {
    buffer->produce(i);
    sleep(1);
  }
  return nullptr;
}

int main(int argc, char **argv) {
  // cv0
  pthread_t pt0;
  pthread_t pt1;
  pthread_create(&pt0, NULL, p0, NULL);
  sleep(1);
  pthread_create(&pt1, NULL, p1, NULL);
  pthread_join(pt0, NULL);
  pthread_join(pt1, NULL);
  // cv1
  pthread_t pt;
  pthread_create(&pt, nullptr, child, nullptr);
  pthread_mutex_lock(&lock);
  while (!done) {
    cout << "main will wait" << endl;
    pthread_cond_wait(&cond, &lock);
    cout << "main is wakeup" << endl;
  }
  pthread_mutex_unlock(&lock);
  // producer/consumer
  /*
  pthread_create(&pt0, NULL, consumer, NULL);
  pthread_create(&pt1, NULL, producer, NULL);
  pthread_join(pt0, NULL);
  pthread_join(pt1, NULL);
  */
  // Buffer
  Buffer buffer(10);
  pthread_create(&pt0, NULL, buffer_consume, &buffer);
  pthread_create(&pt1, NULL, buffer_produce, &buffer);
  pthread_join(pt0, NULL);
  pthread_join(pt1, NULL);
  return EXIT_SUCCESS;
}