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

// insert at head
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

// append at tail
void job_append(Queue *q, Job *job) {
  pthread_rwlock_wrlock(&q->lock);
  job->next = NULL;
  job->prev = q->tail;
  if (q->tail != NULL) {
    q->tail->next = job;
  } else {
    q->head = job;
  }
  q->tail = job;
  pthread_rwlock_unlock(&q->lock);
}

Job *job_find(Queue *q, pthread_t pid) {
  Job *job;
  if (pthread_rwlock_rdlock(&q->lock) != 0) {
    return NULL;
  }
  for (job = q->head; job != NULL; job = job->next) {
    if (pthread_equal(job->pid, pid)) break;
  }
  pthread_rwlock_unlock(&q->lock);
  return job;
}
