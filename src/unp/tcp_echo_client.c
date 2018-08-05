#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>

void str_cli(FILE *fp, int sockfd) {
  char sendline[1500];
  char recvline[1500];
  while (fgets(sendline, 1500, fp) != NULL) {
    write(sockfd, sendline, strlen(sendline));
    read(sockfd, recvline, 1500);
    fputs(recvline, stdout);
  }
}

int main(int argc, char **argv) {
  int sockfd;
  struct sockaddr_in servaddr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8888);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
  str_cli(stdin, sockfd);
  return 0;
}