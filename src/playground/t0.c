//
// Created by Yinyin Qian on 5/30/18.
//
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct __myarg_t {
  int a;
  int b;
} myarg_t;

typedef struct __myret_t {
  int x;
  int y;
} myret_t;

static volatile int counter = 0;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// pthread_mutex_trylock
// pthread_mutex_timedlock
void lock_wrapper(pthread_mutex_t *lk) {
  int rc = pthread_mutex_lock(lk);
  assert(rc == 0);
}

void unlock_wrapper(pthread_mutex_t *lk) {
  int rc = pthread_mutex_unlock(lk);
  assert(rc == 0);
}

void *mythread(void *arg) {
  printf("%s start\n", (char *)arg);
  for (int i = 0; i < 1e4; ++i) {
    lock_wrapper(&lock);
    counter++;
    unlock_wrapper(&lock);
  }
  printf("%s end\n", (char *)arg);
  return NULL;
}

void *mythread2(void *arg) {
  myarg_t *m = (myarg_t *)arg;
  printf("%d %d\n", m->a, m->b);
  myret_t *r = malloc(sizeof(myret_t));
  r->x = 1;
  r->y = 2;
  return (void *)r;
}

int done = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void thr_exit() {
  pthread_mutex_lock(&m);
  done = 1;
  pthread_cond_signal(&c);
  pthread_mutex_unlock(&m);
}

void *child(void *arg) {
  printf("child\n");
  thr_exit();
  return NULL;
}

void thr_join() {
  pthread_mutex_lock(&m);
  while (done == 0) {
    pthread_cond_wait(&c, &m);  // 放弃 m 锁并且等待
  }
  pthread_mutex_unlock(&m);
}

int main(int argc, char **argv) {
  pthread_t p1, p2;
  int rc;
  printf("main: begin\n");
  rc = pthread_create(&p1, NULL, mythread, "A");
  assert(rc == 0);
  rc = pthread_create(&p2, NULL, mythread, "B");
  assert(rc == 0);
  rc = pthread_join(p1, NULL);
  assert(rc == 0);
  rc = pthread_join(p2, NULL);
  assert(rc == 0);
  printf("main: end, counter = %d\n", counter);  // counter != 2e4
  // Thread API
  pthread_t p;
  myarg_t args;
  args.a = 10;
  args.b = 20;
  rc = pthread_create(&p, NULL, mythread2, &args);
  myret_t *m;
  rc = pthread_join(p, (void **)&m);
  printf("returned %d %d\n", m->x, m->y);
  free(m);
  // condition variable
  printf("parent: begin\n");
  pthread_t p3;
  pthread_create(&p3, NULL, child, NULL);
  thr_join();
  printf("parent: end\n");
  return 0;
}