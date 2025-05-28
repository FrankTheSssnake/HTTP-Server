#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void start_ipv4(int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1) {
        perror("Error: ");
        
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind Failed: ");
        
    }

     if (listen(fd, 1) < 0) {
        perror("listen Failed: ");
        
    }

    // accept
    socklen_t addr_len = sizeof(server_addr);
    int client_fd = accept(fd, (struct sockaddr *) &server_addr, &addr_len);

    if (client_fd == -1) {
        perror("accept: ");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // read

    char buffer[1024];
    ssize_t bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_received < 0) {
        perror("read failed");
    } else {
        buffer[bytes_received] = '\0';
        printf("Received: %s\n", buffer);
    }

    close(fd);
}

void start_ipv6(int port) {
    int fd = socket(AF_INET6, SOCK_STREAM, 0);

    if (fd == -1) {
        perror("Error: ");
        
    }

    struct sockaddr_in6 server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin6_family = AF_INET;
    server_addr.sin6_port = htons(port);
    memcpy(&server_addr.sin6_addr, &in6addr_any, sizeof(server_addr.sin6_addr));
    
    if (bind(fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind Failed: ");
        
    }

     if (listen(fd, 1) < 0) {
        perror("listen Failed: ");
        
    }

    // accept
    socklen_t addr_len = sizeof(server_addr);
    int client_fd = accept(fd, (struct sockaddr *) &server_addr, &addr_len);

    if (client_fd == -1) {
        perror("accept: ");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // read

    char buffer[1024];
    ssize_t bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_received < 0) {
        perror("read failed");
    } else {
        buffer[bytes_received] = '\0';
        printf("Received: %s\n", buffer);
    }

    close(fd);

}

void start(int ipv4, int ipv6, int port) {

    if (ipv4 == 1) {
        start_ipv4(port);
    } else if (ipv6 == 1) {
        start_ipv6(port);
    }

}
