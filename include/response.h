#ifndef RESPONSE_H
#define RESPONSE_H

#include <stdint.h>
#include <stdlib.h>

#include "request.h"

typedef struct {
    uint16_t status_code;
    const char *status_msg;
    size_t content_length;
    char *raw_header;
} Header;

typedef struct {
    Header header;
    char *body;
    char *raw;
} HttpResponse;


HttpResponse *init_response();

int send_response(int sock_fd, HttpRequest request);

#endif
