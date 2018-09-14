#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void str_echo(int sockfd) {
  ssize_t n;
  char buf[1500];
  while ((n = read(sockfd, buf, 1500)) > 0) {
    write(sockfd, buf, n);
  }
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
  servaddr.sin_port = htons(8888); // byte ordering func, host to network, short
  bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)); // 把端口绑定到 socket
  listen(listenfd, 1024); // 转化为 listening socket，1024 是最大等待数
  for (;;) {
    chilen = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &chilen); // 客户端三次握手完成后，accept 返回
    if ((childpid = fork()) == 0) { // fork 子进程
      close(listenfd); // 子进程关闭 listenfd，引用数减一
      str_echo(connfd); // business logic
      exit(0); // 子进程退出
    }
    close(connfd); // 父进程关闭 connfd，引用数减一
  }
}