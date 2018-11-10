struct lock_t {
  int flag;
};

void init(lock_t *lock) {
  lock->flag = 0;
}


void unlock(lock_t *lock) {
  lock->flag = 0;
}

// TAS
int TestAndSet(int *old_ptr, int newVal) {
  int old = *old_ptr;
  *old_ptr = newVal;
  return old;
}

void tasLock(lock_t *lock) {
  while (TestAndSet(&lock->flag, 1) == 1)
    ; //sping-wait
}

// CAS, IMPORTANT!!!
int CompareAndSwap(int *ptr, int expected, int newVal) {
  int actual = *ptr;
  if (actual == expected) *ptr = newVal;
  return actual;
}

void casLock(lock_t *lock) {
  while (CompareAndSwap(&lock->flag, 0, 1) == 1)
    ; // 0 表示没有锁住，1表示锁住
}

// FetchAndAdd
struct TicketLock {
  int ticket;
  int turn;
};

int FetchAndAdd(int *ptr) {
  int old = *ptr;
  *ptr = old + 1;
  return old;
}

void ticketInit(TicketLock *lock) {
  lock->ticket = 0;
  lock->turn = 0;
}

void ticketLock(TicketLock *lock) {
  int myturn = FetchAndAdd(&lock->ticket);
  while (lock->turn != myturn)
    ; // spin
}

void ticketUnLock(TicketLock *lock) {
  lock->turn = lock->turn + 1;
}
