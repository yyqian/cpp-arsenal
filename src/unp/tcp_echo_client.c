#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

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
  char sendline[MAX_BUFF_SIZE];
  char recvline[MAX_BUFF_SIZE];
  while (fgets(sendline, MAX_BUFF_SIZE, fp) != NULL) {
    size_t len = strlen(sendline);
    if (len == 1) break; // only /r or /n
    print(sendline, len);
    write(sockfd, sendline, len);
    ssize_t n = read(sockfd, recvline, MAX_BUFF_SIZE);
    print(recvline, n);
    write(STDIN_FILENO, recvline, n); // stdout, stdio.h
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