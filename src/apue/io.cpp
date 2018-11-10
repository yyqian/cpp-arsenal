#include <unistd.h>
#include <stdio.h>

void pipe_test() {
  int n;
  int fd_in[2];
  int fd_out[2];
  pid_t pid;
  char line[1000];
  if (pipe(fd_in) < 0) printf("pip failed");
  if (pipe(fd_out) < 0) printf("pip failed");
  if ((pid = fork()) < 0) {
    printf("fork failed");
  } else if (pid > 0) {
    close(fd_in[0]);
    close(fd_out[1]);
    write(fd_in[1], "Hello world\n", 12);
    n = read(fd_out[0], line, 1000);
    write(STDOUT_FILENO, line, n);
  } else {
    close(fd_in[1]);
    close(fd_out[0]);
    n = read(fd_in[0], line, 1000);
    write(fd_out[1], line, n);
  }
}

void popen_test() {
  FILE *fp = popen("ls -al", "r");
  char buf[1000];
  while(fgets(buf, 1000, fp) != NULL) {
    fputs(buf, stdout);
  }
}

int main(int argc, char* argv[]) {
  popen_test();
  return 0;
}