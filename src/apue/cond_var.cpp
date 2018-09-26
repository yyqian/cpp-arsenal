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
  pthread_create(&pt0, NULL, consumer, NULL);
  pthread_create(&pt1, NULL, producer, NULL);
  pthread_join(pt0, NULL);
  pthread_join(pt1, NULL);
  return EXIT_SUCCESS;
}