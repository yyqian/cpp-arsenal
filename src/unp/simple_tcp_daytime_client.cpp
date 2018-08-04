#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>

#define MAXLINE 1000

int main(int argc, char **argv) {
  int sockfd;
  int n;
  char buff[MAXLINE + 1];
  struct sockaddr_in servaddr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0); // 创建 TCP socket
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8888);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr); // ip 转换
  connect(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)); // 创建连接
  while ((n = read(sockfd, buff, MAXLINE)) > 0) { // 读取和打印
    buff[n] = 0; // null terminate
    fputs(buff, stdout);
  }
  return 0;
}