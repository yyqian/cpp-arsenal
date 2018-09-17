#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/select.h>

#define SERVER_PORT 8888
#define MAX_BUFF_SIZE 10

void print(char *str, int n) {
  printf("raw data: ");
  for (int i = 0; i < n; ++i) {
    char c = str[i];
    printf("%d ", c);
  }
  printf("\n");
}

int main(int argc, char **argv) {
  char buf[MAX_BUFF_SIZE];

  int listenfd = socket(AF_INET, SOCK_STREAM, 0); // 创建 TCP socket

  struct sockaddr_in cliaddr, servaddr; // <netinet/in.h>
  bzero(&servaddr, sizeof(servaddr)); // <string.h>, can be replaced with memset
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // byte ordering func, host to network, long
  servaddr.sin_port = htons(SERVER_PORT); // byte ordering func, host to network, short

  bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)); // 把端口绑定到 socket

  listen(listenfd, 1024); // 转化为 listening socket，1024 是最大等待数
  printf("listenfd: %d\n", listenfd);

  int client[FD_SETSIZE]; // 1024
  int maxfd = listenfd;
  int maxi = -1;
  for (int i = 0; i < FD_SETSIZE; ++i) {
    client[i] = -1;
  }
  fd_set rset, allset;
  FD_ZERO(&allset);
  FD_SET(listenfd, &allset);

  for (;;) {
    rset = allset;
    int ready = select(maxfd + 1, &rset, NULL, NULL, NULL);  // block here
    printf("select [%d]\n", ready);
    if (FD_ISSET(listenfd, &rset)) { // listening socket is readable
      socklen_t children = sizeof(cliaddr);
      int connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &children); // BAD: may blocking
      printf("accept [%d]\n", connfd);
      int i = 0;
      for (; i < FD_SETSIZE; ++i) {
        if (client[i] < 0) {
          client[i] = connfd;
          break;
        }
      }
      FD_SET(connfd, &allset); // add new fd
      if (connfd > maxfd) maxfd = connfd;
      if (i > maxi) maxi = i;
      if (--ready <= 0) continue;
    }
    for (int i = 0; i <= maxi; ++i) {
      int sockfd = client[i];
      if (sockfd < 0) continue;
      if (FD_ISSET(sockfd, &rset)) {
        int n = read(sockfd, buf, MAX_BUFF_SIZE); // if buf size is not big enough, will select again, BAD: may blocking
        printf("read [%d] from %d\n", n, sockfd);
        if (n == 0) {
          printf("client closed\n");
          close(sockfd);
          FD_CLR(sockfd, &allset);
          client[i] = -1;
        } else {
          print(buf, n);
          write(sockfd, buf, n);
        }
        if (--ready <= 0) break;
      }
    }
  }
}