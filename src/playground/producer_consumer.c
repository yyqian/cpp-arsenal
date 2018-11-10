//
// Created by Yinyin Qian on 5/31/18.
//
#include <pthread.h>
#include <stdio.h>

static const int MAX = 100;

int buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;

void put(int val) {
  buffer[fill_ptr] = val;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;
}

int get() {
  int tmp = buffer[use_ptr];
  use_ptr = (use_ptr + 1) % MAX;
  count--;
  return tmp;
}

pthread_cond_t empty, fill;
pthread_mutex_t mutex;

void *producer(void *arg) {
  for (int i = 0; i < 100; ++i) {
    pthread_mutex_lock(&mutex);
    while (count == MAX) {
      pthread_cond_wait(&empty, &mutex);
    }
    put(i);
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&mutex);
  }
}

void *consumer(void *arg) {
  for (int i = 0; i < 1000; ++i) {
    pthread_mutex_lock(&mutex);
    while (count == 0) {
      pthread_cond_wait(&fill, &mutex);
    }
    int tmp = get();
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);
    printf("%d\n", tmp);
  }
}

int main() {
  pthread_t p1, p2, c1, c2;
  pthread_create(&p1, NULL, producer, NULL);
  pthread_create(&p2, NULL, producer, NULL);
  pthread_create(&c1, NULL, consumer, NULL);
  pthread_create(&c2, NULL, consumer, NULL);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  pthread_join(c1, NULL);
  pthread_join(c2, NULL);
  return 0;
}