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
  struct sockaddr_in cliaddr, servaddr;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(8888);
  bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
  listen(listenfd, 1024);
  for (;;) {
    chilen = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &chilen);
    if ((childpid = fork()) == 0) {
      close(listenfd);
      str_echo(connfd);// business logic
      exit(0);
    }
    close(connfd);
  }
}