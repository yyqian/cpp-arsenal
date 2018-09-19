#include <stdio.h>
#include <pthread.h>

struct Job {
  Job *prev;
  Job *next;
  pthread_t pid;
};

struct Queue {
  Job *head;
  Job *tail;
  pthread_rwlock_t lock;
};

int queue_init(Queue *q) {
  q->head = NULL;
  q->tail = NULL;
  pthread_rwlock_init(&q->lock, NULL);
  return 0;
}

void job_insert(Queue *q, Job *job) {
  pthread_rwlock_wrlock(&q->lock);
  job->next = q->head;
  job->prev = NULL;
  if (q->head != NULL) {
    q->head->prev = job;
  } else {
    q->tail = job;
  }
  q->head = job;
  pthread_rwlock_unlock(&q->lock);
}
