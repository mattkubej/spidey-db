#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>

#define PORT 8080
#define TCP_BACKLOG 11

int main() {
  int serv_fd, in_fd, val_read;
  struct sockaddr_in serv_addr, in_addr;
  int in_len = sizeof(in_addr);

  char *pong = "PONG\n";

  if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("failed to create socket");
    exit(EXIT_FAILURE);
  }

  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);

  if (bind(serv_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("failed to bind socket");
      exit(EXIT_FAILURE);
  }

  if (listen(serv_fd, TCP_BACKLOG) < 0) {
    perror("failed to listen");
    exit(EXIT_FAILURE);
  }

  while (1) {
    printf("--- listening for connections ---\n\n");

    if ((in_fd = accept(serv_fd, (struct sockaddr*) &in_addr, (socklen_t*) &in_len)) < 0) {
      perror("failed to accept");
      exit(EXIT_FAILURE);
    }

    char buffer[30000] = {0};
    val_read = read(in_fd, buffer, 30000);
    printf("%s\n", buffer);

    write(in_fd, pong, strlen(pong));
    close(in_fd);
  }

  return 0;
}
