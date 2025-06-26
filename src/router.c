#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <asm-generic/errno-base.h>

#include "../include/router.h"
#include "../include/util.h"

Route routes[] = {
    { GET, get_or_head },
    { HEAD, get_or_head },
    { CONNECT, not_supported },
    { DELETE, not_supported },
    { OPTIONS, not_supported },
    { PATCH, not_supported },
    { POST, not_supported },
    { PUT, not_supported },
    { TRACE, not_supported },
    { INVALID, invalid }
};


char *route_request(HttpRequest *request, HttpResponse *response) {
    for (int i = 0; routes[i].method != INVALID; i++) {
        if (routes[i].method == request->method) {
            printf("[DEBUG] Handling method: %d with %p\n", request->method, routes[i].handler);
            return routes[i].handler(request, response);
        }
    }

    response->header.status_code = 404;
    return NULL;
}


char *get_or_head(HttpRequest *request, HttpResponse *response) {
    char path[512];
    if (getcwd(path, sizeof(path)) == NULL) {
        response->header.status_code = 500;
        fprintf(stderr, "[ERROR] Could not get current working directory.\n");
        return NULL;
    }

    if (strlen(path) + strlen(request->path) >= sizeof(path)) {
        response->header.status_code = 500;
        fprintf(stderr, "[ERROR] Path too long.\n");
        return NULL;
    }

    strcat(path, request->path);

    FILE *fp = fopen(path, "r");

    if (fp == NULL) {
        switch (errno) {
            case EACCES:
                response->header.status_code = 403;
                break;
            case ENOENT:
                response->header.status_code = 404;
                break;
            case EISDIR:
                response->header.status_code = 422;
                break;
            case ETXTBSY:
                response->header.status_code = 503;
                break;
            default:
                response->header.status_code = 500;
                break;
        }
        return NULL;
    }

    int size = get_file_size(fp);

    char *body = malloc(size + 1);
    if (!(body)) {
        response->header.status_code = 500;
        fprintf(stderr, "[ERROR] Could not allocate memory.\n");
        fclose(fp);
        return NULL;
    }

    response->header.status_code = 200;
    response->header.content_length = size;

    if (request->method == HEAD) {
        body = NULL;
    } else {
        size_t bytes_read = fread(body, 1, size, fp);
        if (bytes_read == 0 && !feof(fp)) {
            response->header.status_code = 500;
            strcpy(body, "");
        } else {
            body[bytes_read] = '\0';
        }
    }

    fclose(fp);
    return body;
}


char *invalid(HttpRequest *request, HttpResponse *response) {
    response->header.status_code = 400;
    
    char *error = "{"
          "\t\"error\": \"Bad request\","
          "\t\"message\": \"Request body could not be read properly.\","
        "}";

    size_t len = strlen(error);

    char *body = malloc(len + 1);
    if (!body) {
        response->header.status_code = 500;
        fprintf(stderr, "[ERROR] Could not allocate memory.\n");
        return NULL;
    }

    strcpy(body, error);

    return body;
}


char *not_supported(HttpRequest *request, HttpResponse *response) {
    response->header.status_code = 501;
    
    char *body = NULL;

    return body;
}
