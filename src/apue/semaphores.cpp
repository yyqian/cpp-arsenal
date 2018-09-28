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
  return EXIT_SUCCESS; // cstdlib
}
