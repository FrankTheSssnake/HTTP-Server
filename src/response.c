#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>

#include "../include/response.h"
#include "../include/request.h"


char *form_header(HttpRequest request) {
    char header[256];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/plain\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n", body_length);
    return header;
}


char *form_body(HttpRequest request) {
    char *body = "Content-Length: 13\r\nContent-Type: text/plain\r\n\r\nHello, world!\r\n";
    return body;
}


char *form_response(HttpRequest request) {
    switch (request.method) {
        case GET:
            printf("[DEBUG] GET Method.\n");
            break;
        case POST:
            printf("[DEBUG] POST Method.\n");
            break;
        default:
            fprintf(stderr, "[ERROR] Method not supported: %d.\n", request.method);
            exit(1);
            break;
    }
    const char *body = "Hello, world!\n";
    int body_length = strlen(body);



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


int send_response(int client_fd, HttpRequest request) {
    char *response = form_response(request);

    if (send(client_fd, response, strlen(response), 0) < 0) {
        perror("[ERROR] send Failed: ");
        free(response);
        return -1;
    }

    free(response);
    return 0;
}
