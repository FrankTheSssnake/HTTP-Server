#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>

#include "../include/response.h"


char *form_header(char *request) {
    char *header = "HTTP/1.1 200 OK\r\n";
    return header;
}


char *form_body(char *request) {
    char *body = "Content-Length: 13\r\nContent-Type: text/plain\r\n\r\nHello, world!\r\n";
    return body;
}


char *form_response(char *request) {
    const char *body = "Hello, world!\n";
    int body_length = strlen(body);

    char header[256];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/plain\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n", body_length);

    size_t total_len = strlen(header) + body_length + 1;
    char *response = malloc(total_len);
    if (!response) {
        perror("[ERROR] malloc Failed: ");
        exit(1);
    }

    strcpy(response, header);
    strcat(response, body);

    return response;
}


int send_response(int sock_fd, char *request) {
    char *response = form_response(request);

    if (send(sock_fd, response, strlen(response), 0) < 0) {
        perror("[ERROR] send Failed: ");
        free(response);
        return -1;
    }

    free(response);
    return 0;
}
