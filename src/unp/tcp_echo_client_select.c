#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define MAX_BUFF_SIZE 100

void print(char *str, int n) {
  printf("raw data: ");
  for (int i = 0; i < n; ++i) {
    char c = str[i];
    printf("%d ", c);
  }
  printf("\n");
}

void str_cli(FILE *fp, int sockfd) {
  int filefd = fileno(fp);
  int maxfdp1 = (filefd > sockfd ? filefd : sockfd) + 1; // not # of fds
  int stdineof = 0;
  fd_set rset; // sys/select.h
  char buf[MAX_BUFF_SIZE];
  int n;
  FD_ZERO(&rset);
  for(;;) {
    if (stdineof == 0) FD_SET(filefd, &rset);// sys/select.h, fileno is in stdio.h
    FD_SET(sockfd, &rset);
    select(maxfdp1, &rset, NULL, NULL, NULL); // block here, wait forever, only interesting in read event
    if (FD_ISSET(sockfd, &rset)) {
      if ((n = read(sockfd, buf, MAX_BUFF_SIZE)) == 0) { // receive echo msg
        if (stdineof == 1) {
          printf("client will exist\n");
        }
        else {
          printf("server is crashed.\n");
        }
        return;
      }
      write(STDOUT_FILENO, "|", 1);
      write(STDOUT_FILENO, buf, n); // print echo msg
    }
    if (FD_ISSET(filefd, &rset)) {
      if ((n = read(filefd, buf, MAX_BUFF_SIZE)) == 1) { // receive user inputs
        stdineof = 1;
        shutdown(sockfd, SHUT_WR);
        FD_CLR(filefd, &rset);
        continue;
      }
      write(sockfd, buf, n); // send msg
    }
  }
}

int main(int argc, char **argv) {
  int sockfd;
  struct sockaddr_in servaddr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8888);
  inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
  connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
  str_cli(stdin, sockfd); // stdin, stdio.h
  return 0;
}
