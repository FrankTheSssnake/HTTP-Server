#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>

#include "../include/request.h"


METHOD get_method(const char *method) {
    if (strcmp(method, "CONNECT") == 0) return CONNECT;
    if (strcmp(method, "DELETE")  == 0) return DELETE;
    if (strcmp(method, "GET")     == 0) return GET;
    if (strcmp(method, "HEAD")    == 0) return HEAD;
    if (strcmp(method, "OPTIONS") == 0) return OPTIONS;
    if (strcmp(method, "PATCH")   == 0) return PATCH;
    if (strcmp(method, "POST")    == 0) return POST;
    if (strcmp(method, "PUT")     == 0) return PUT;
    if (strcmp(method, "TRACE")   == 0) return TRACE;

    return INVALID;
}

HttpRequest parse_request(const char *request) {
    HttpRequest req = {0};

    char buffer[1024];
    strncpy(buffer, request, sizeof(buffer) - 1);

    char *line = strtok(buffer, "\r\n");
    if (!line) goto invalid;

    char *method = strtok(line, " ");
    char *path = strtok(NULL, " ");
    char *version = strtok(NULL, " ");

    if ((method) && (path) && (version)) {
        req.method = get_method(method);
        strncpy(req.path, path, sizeof(req.path) - 1);
        strncpy(req.version, version, sizeof(req.version) - 1);
        return req;
    } else goto invalid;

    invalid:
        fprintf(stderr, "[ERROR] Invalid Request.\n");
        exit(1);
}


void print_info(HttpRequest request) {
    printf("[INFO] %d %s %s\n", request.method, request.path, request.version);
}
