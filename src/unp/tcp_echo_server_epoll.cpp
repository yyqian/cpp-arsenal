#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER_PORT 8888
#define MAX_BUFF_SIZE 10
#define MAX_EVENTS 10

void print(char *str, int n) {
  printf("raw data: ");
  for (int i = 0; i < n; ++i) {
    char c = str[i];
    printf("%d ", c);
  }
  printf("\n");
}

void check(int ret, const char *msg) {
  if (ret < 0) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

void set_fl(int fd, int flags) {
    int opts;
    check(opts = fcntl(fd, F_GETFL), "fcntl(sock, GETFL)");
    opts |= flags;
    check(fcntl(fd, F_SETFL, opts), "fcntl(sock, SETFL, opts)");
}

void echo(int fd) {
  char buf[MAX_BUFF_SIZE];
  while(true) { // 边缘触发需要把数据都读掉，直到 EAGAIN
    int n = read(fd, buf, MAX_BUFF_SIZE);
    printf("read from %d: n=%d, errno=%d\n", fd, n, n < 0 ? errno : 0);
    if (n == 0) { // closing fd
      printf("client closed\n");
      close(fd);
      break;
    } else if (n < 0) {
      if (errno == EAGAIN) { // int <errno.h>
        printf("EAGAIN\n");
      } else {
        printf("errno: %d\n", errno);
      }
      break;
    } else {
      print(buf, n);
      write(fd, buf, n);
      if (n < MAX_BUFF_SIZE) break; // it means no more data
    }
  }
  printf("echo exit\n");
}

int main(int argc, char **argv) {
  char buf[MAX_BUFF_SIZE];

  int listenfd = socket(AF_INET, SOCK_STREAM, 0); // 创建 TCP socket

  struct sockaddr_in cliaddr, servaddr; // <netinet/in.h>
  socklen_t addrlen = sizeof(cliaddr);
  bzero(&servaddr, sizeof(servaddr)); // <string.h>, can be replaced with memset
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // byte ordering func, host to network, long
  servaddr.sin_port = htons(SERVER_PORT); // byte ordering func, host to network, short

  bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)); // 把端口绑定到 socket

  listen(listenfd, 1024); // 转化为 listening socket，1024 是最大等待数
  printf("listenfd: %d\n", listenfd);

  int epollfd = epoll_create1(0);
  check(epollfd, "epoll_create1");

  struct epoll_event ev, events[MAX_EVENTS];
  ev.events = EPOLLIN;
  ev.data.fd = listenfd;
  int stat = epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);
  check(stat, "epoll_ctl: listen_sock");

  while(true) {
    int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1); // 等待epoll事件的发生
    check(nfds, "epoll_wait");
    for (int i = 0; i < nfds; ++i) {
      if (events[i].data.fd == listenfd) { //如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接
        int connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &addrlen);
        check(connfd, "accept");
        printf("client connected, ip[%s], fd[%d]\n", inet_ntoa(cliaddr.sin_addr), connfd);
        set_fl(connfd, O_NONBLOCK); // nonblock fd, without this, read(connfd) will block if no data 边缘触发需要设置文件句柄为非阻塞
        ev.events = EPOLLIN | EPOLLET; //边缘触发
        ev.data.fd = connfd;
        check(epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev), "epoll_ctl: conn_sock"); //注册 event
      } else if (events[i].events & EPOLLIN) {
        echo(events[i].data.fd);
      }
    }
  }
}
