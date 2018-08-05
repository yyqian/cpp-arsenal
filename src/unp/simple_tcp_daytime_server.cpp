#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define MAXLINE 1000

int main(int argc, char **argv) {
  int listenfd;
  int connfd;
  pid_t pid;
  socklen_t len;
  struct sockaddr_in servaddr, cliaddr;
  char buff[MAXLINE];
  time_t ticks;
  listenfd = socket(AF_INET, SOCK_STREAM, 0); // 创建 TCP socket
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(8888);
  bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)); // 把端口绑定到 socket
  listen(listenfd, 1024); // 转化为 listening socket，1024 是最大等待数
  while (true) {
    connfd = accept(listenfd, (sockaddr *) &cliaddr, &len); // 客户端三次握手完成后，accept 返回
    if ((pid = fork()) == 0) {
      close(listenfd);
      printf("connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
      ticks = time(NULL);
      snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks)); // snprintf 代替 sprintf
      write(connfd, buff, strlen(buff));
      close(connfd); // 服务端断开连接，这句可有可无，因为下一句就进程退出了
      exit(0); // 子进程退出
    }
    close(connfd); // 父进程 call close，这个时候只是引用数减一，不真正四次挥手
  }
  return 0;
}