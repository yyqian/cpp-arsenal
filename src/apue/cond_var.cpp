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
  return EXIT_SUCCESS;
}