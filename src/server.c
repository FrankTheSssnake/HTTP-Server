#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "../include/response.h"


void start_ipv4(int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1) {
        perror("Error: ");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind Failed: ");
        exit(EXIT_FAILURE);
    }

    printf("[DEBUG] binded to: %d:%d\n", server_addr.sin_addr.s_addr, server_addr.sin_port);

    if (listen(fd, 1) < 0) {
        perror("listen Failed: ");
        exit(EXIT_FAILURE);
    }
    
    printf("[DEBUG] Listening...\n");

    while (1) {

        socklen_t addr_len = sizeof(server_addr);
        int client_fd = accept(fd, (struct sockaddr *) &server_addr, &addr_len);

        if (client_fd == -1) {
            perror("accept Failed: ");
            close(client_fd);
            break;
        }

        printf("[DEBUG] Accepted connection.\n");

        // read
        char response[1024];
        ssize_t bytes_received = read(client_fd, response, sizeof(response) - 1);

        if (bytes_received < 0) {
            perror("read Failed: ");
            break;
        } else {
            response[bytes_received] = '\0';
            printf("Received: %s\n", response);
            
            send_response(client_fd, response);
        }

        printf("[DEUBG] Respone sent.\n");
    }

    close(fd);
}

void start_ipv6(int port) {
    int fd = socket(AF_INET6, SOCK_STREAM, 0);

    if (fd == -1) {
        perror("Error: ");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in6 server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(port);
    memcpy(&server_addr.sin6_addr, &in6addr_any, sizeof(server_addr.sin6_addr));

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind Failed: ");
        exit(EXIT_FAILURE);
    }

    if (listen(fd, 1) < 0) {
        perror("listen Failed: ");
        exit(EXIT_FAILURE);
    }

    while (1) {

        socklen_t addr_len = sizeof(server_addr);
        int client_fd = accept(fd, (struct sockaddr *) &server_addr, &addr_len);

        if (client_fd == -1) {
            perror("accept Failed: ");
            close(client_fd);
            break;
        }

        // read
        char response[1024];
        ssize_t bytes_received = read(client_fd, response, sizeof(response) - 1);

        if (bytes_received < 0) {
            perror("read Failed: ");
            break;
        } else {
            response[bytes_received] = '\0';
            printf("Received: %s\n", response);
        }
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
