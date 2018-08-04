#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>

#define MAXLINE 1000

int main(int argc, char **argv) {
  int listenfd;
  int connfd;
  struct sockaddr_in servaddr;
  char buff[MAXLINE];
  time_t ticks;
  listenfd = socket(AF_INET, SOCK_STREAM, 0); // 创建 TCP socket
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(8888);
  bind(listenfd, (sockaddr *)&servaddr, sizeof(servaddr)); // 把端口绑定到 socket
  listen(listenfd, 1024); // 转化为 listening socket，1024 是最大等待数
  while (true) {
    connfd = accept(listenfd, (sockaddr *) NULL, NULL); // 客户端三次握手完成后，accept 返回
    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks)); // snprintf 代替 sprintf
    write(connfd, buff, strlen(buff));
    close(connfd); // 服务端断开连接
  }
  return 0;
}