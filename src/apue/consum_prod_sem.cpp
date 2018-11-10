#include <semaphore.h>
#include <cstdlib>
#include <iostream>

using namespace std;

void *child(void *arg) {
  sem_t *sem = (sem_t *)arg;
  cout << "child" << endl;
  sem_post(sem);
  return NULL;
}

#define MAX 10
sem_t mutex;
sem_t not_full;
sem_t not_empty;
int qdata[MAX];
int front = 0;
int back = 0;

void *producer(void *arg) {
  for (int i = 0; i < 50; ++i) {
    sem_wait(&not_full);
    sem_wait(&mutex);
    qdata[back] = i;
    back = (back + 1) % MAX; // keypoint
    cout << "put:" << i << endl;
    sem_post(&mutex);
    sem_post(&not_empty);
  }
}

void *consumer(void *arg) {
  for (int i = 0; i < 50; ++i) {
    sem_wait(&not_empty);
    sem_wait(&mutex);
    cout << "get:" << qdata[front] << endl;
    front = (front + 1) % MAX; // keypoint
    sem_post(&mutex);
    sem_post(&not_full);
  }
}

int main(int argc, char **argv) {
  sem_t sem;
  sem_init(&sem, 0, 1);
  sem_wait(&sem);
  sem_post(&sem);
  // ordering
  sem_t sem1;
  sem_init(&sem1, 0, 0);
  cout << "parent: begin" << endl;
  pthread_t pt;
  pthread_create(&pt, NULL, child, &sem1);
  sem_wait(&sem1);
  cout << "parent: end" << endl;
  //
  sem_init(&mutex, 0, 1); // mutex
  sem_init(&not_full, 0, MAX);
  sem_init(&not_empty, 0, 0);
  pthread_t t0;
  pthread_t t1;
  pthread_create(&t0, NULL, producer, NULL);
  pthread_create(&t1, NULL, consumer, NULL);
  pthread_join(t0, NULL);
  pthread_join(t1, NULL);
  return EXIT_SUCCESS; // cstdlib
}
