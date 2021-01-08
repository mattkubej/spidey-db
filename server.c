#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#define PORT 8080

int main() {

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("failed to create socket\n");
    exit(1);
  }

  struct sockaddr_in servaddr;
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  int err = bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
  if (err != 0) {
      printf("failed to bind socket\n");
      return err;
  }

  if (listen(sockfd, 511) != 0) {
    printf("failed to listen\n");
    exit(1);
  }

  struct sockaddr in_addr;
  socklen_t in_len = sizeof(in_addr);

  int in_fd = accept(sockfd, &in_addr, &in_len);
  if (in_fd < 0) {
    printf("failed to accept");
    exit(0);
  }

  return 0;
}
