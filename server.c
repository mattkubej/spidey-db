#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#define PORT 8080
#define TCP_BACKLOG 511

int main() {
  int sockfd, in_fd;
  struct sockaddr_in servaddr;
  struct sockaddr in_addr;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("failed to create socket");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
      perror("failed to bind socket");
      exit(EXIT_FAILURE);
  }

  if (listen(sockfd, TCP_BACKLOG) < 0) {
    perror("failed to listen");
    exit(EXIT_FAILURE);
  }

  socklen_t in_len = sizeof(in_addr);

  if ((in_fd = accept(sockfd, &in_addr, &in_len)) < 0) {
    perror("failed to accept");
    exit(EXIT_FAILURE);
  }

  return 0;
}
