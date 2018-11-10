#include <cstdlib>
#include <stdio.h>
#include <pthread.h>
#include <vector>
#include <unistd.h>

#define THREAD_NUM 10

using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x): val(x), left(nullptr), right(nullptr) {}
};

void *thr_fn(void *arg) {
  TreeNode *root = (TreeNode *)arg;
  printf("new thread: %lu 0x%lx, with root %d, left %d, right %d\n", pthread_self(), pthread_self(), root->val, root->left->val, root->right->val);
  TreeNode *aux = root->left;
  root->left = root->right;
  root->right = aux;
  return root;
}

struct SafeCount {
  int val;
  pthread_mutex_t lock;
  SafeCount(): val(0) {
    pthread_mutex_init(&lock, NULL);
  }
};

void *inc(void *arg) {
  SafeCount *cnt = (SafeCount *)arg;
  for (int i = 0; i < 100000; ++i) {
    pthread_mutex_lock(&(cnt->lock));
    cnt->val++;
    pthread_mutex_unlock(&(cnt->lock));
  }
  return NULL;
}

struct AutoObj {
  int id;
  int ref;
  pthread_mutex_t lock;
  AutoObj(int x): id(x), ref(1) {
    pthread_mutex_init(&lock, NULL);
  }
};
void hold(AutoObj *obj) {
  pthread_mutex_lock(&obj->lock);
  obj->ref++;
  printf("ref: %d\n", obj->ref);
  pthread_mutex_unlock(&obj->lock);
}
void release(AutoObj *obj) {
  pthread_mutex_lock(&obj->lock);
  if (--obj->ref == 0) {
    printf("ref: %d\n", obj->ref);
    pthread_mutex_unlock(&obj->lock);
    pthread_mutex_destroy(&obj->lock);
    free(obj);
    printf("AutoObj* is free\n");
  } else {
    printf("ref: %d\n", obj->ref);
    pthread_mutex_unlock(&obj->lock);
  }
}

void *holdAndRelObj(void *arg) {
  AutoObj *obj = (AutoObj *)arg;
  hold(obj);
  sleep(1);
  release(obj);
  return NULL;
}

int main() {
  printf("current thread: %lu 0x%lx\n", pthread_self(), pthread_self());
  TreeNode *root = new TreeNode(3);
  root->left = new TreeNode(1);
  root->right = new TreeNode(5);
  pthread_t pid0, pid1, pid2;
  vector<pthread_t> pid_vec(THREAD_NUM);
  TreeNode *ret;
  pthread_create(&pid0, NULL, thr_fn, root);
  pthread_join(pid0, (void **)&ret);
  printf("return val: root %d, left %d, right %d\n", ret->val, ret->left->val, ret->right->val);
  SafeCount *cnt = new SafeCount();
  for (int i = 0; i < THREAD_NUM; ++i) {
    pthread_create(&(pid_vec[i]), NULL, inc, cnt);
  }
  for (int i = 0; i < THREAD_NUM; ++i) {
    pthread_join(pid_vec[i], NULL);
  }
  printf("cnt = %d\n", cnt->val);
  AutoObj *obj = new AutoObj(3);
  for (int i = 0; i < THREAD_NUM; ++i) {
    pthread_create(&(pid_vec[i]), NULL, holdAndRelObj, obj);
  }
  for (int i = 0; i < THREAD_NUM; ++i) {
    pthread_join(pid_vec[i], NULL);
  }
  release(obj);
  return EXIT_SUCCESS;
}