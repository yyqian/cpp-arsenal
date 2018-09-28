#include <semaphore.h>
#include <cstdlib>


int main(int argc, char **argv) {
  sem_t sem;
  sem_init(&sem, 0, 1);
  sem_wait(&sem);
  sem_post(&sem);
  return EXIT_SUCCESS; // cstdlib
}