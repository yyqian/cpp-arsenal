#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define SERVER_PORT 8888
#define MAX_BUFF_SIZE 100
char global_buf[MAX_BUFF_SIZE];

void print(char *str, int n) {
  printf("raw data: ");
  for (int i = 0; i < n; ++i) {
    char c = str[i];
    printf("%d ", c);
  }
  printf("\n");
}

void echo(int connfd) {
  ssize_t n;
  char buf[MAX_BUFF_SIZE];
  while ((n = read(connfd, buf, MAX_BUFF_SIZE)) > 0) { // will block, if the connection is FIN, n == 0
    print(buf, n);
    write(connfd, buf, n);
  }
}

const char *read_oneline(int connfd) {
  ssize_t n = read(connfd, global_buf, MAX_BUFF_SIZE);
  write(STDOUT_FILENO, "received: ", 10); // STDOUT_FILENO is part of unistd.h
  for (int i = 0; i < n; ++i) {
    char c = global_buf[i];
    printf("%d ", c);
    if (c == '\r' || c == '\n') {
      global_buf[i] = '\0';
    }
  }
  printf("\n");
  return (const char *)global_buf;
}

void print_file(int connfd, const char *filename) {
  printf("reading file: [%s]\n", filename);
  int filefd = open(filename, O_RDONLY);// <fcntl.h> | open is system call, low level, not portable | fopen is c lib function, portable, high level, buffer io
  if (filefd <= 0) {
    printf("File not found\n");
  }
  char buf[MAX_BUFF_SIZE];
  ssize_t n;
  while ((n = read(filefd, buf, MAX_BUFF_SIZE)) > 0) {
    printf(".");
    write(connfd, buf, n);
  }
  printf("\n");
  close(filefd);
}

void process_connfd(int connfd) {
  echo(connfd);
}

int main(int argc, char **argv) {
  int listenfd, connfd;
  pid_t childpid;
  socklen_t chilen;
  struct sockaddr_in cliaddr, servaddr; // <netinet/in.h>
  listenfd = socket(AF_INET, SOCK_STREAM, 0); // 创建 TCP socket
  bzero(&servaddr, sizeof(servaddr)); // <string.h>, can be replaced with memset
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // byte ordering func, host to network, long
  servaddr.sin_port = htons(SERVER_PORT); // byte ordering func, host to network, short
  bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)); // 把端口绑定到 socket
  listen(listenfd, 1024); // 转化为 listening socket，1024 是最大等待数
  for (;;) {
    chilen = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &chilen); // the server will block here, 客户端三次握手完成后，accept 返回
    printf("Three-way handshake finished. Connection is established.\n");
    if ((childpid = fork()) == 0) { // fork 子进程
      printf("Child process will close listenfd\n");
      close(listenfd); // 子进程关闭 listenfd，引用数减一
      process_connfd(connfd); // business logic
      printf("Child process will exit\n");
      exit(0); // 子进程退出
    }
    printf("Parent process will close connfd\n");
    close(connfd); // 父进程关闭 connfd，引用数减一
  }
}