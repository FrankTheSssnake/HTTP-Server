#ifndef REQUEST_H
#define REQUEST_H

#include <stdint.h>
typedef enum METHOD {
    CONNECT,
    DELETE,
    GET,
    HEAD,
    OPTIONS,
    PATCH,
    POST,
    PUT,
    TRACE,
    INVALID
} METHOD;

typedef struct {
    METHOD method;
    char path[256];
    char version[16];
    char body[1024];
} HttpRequest;


HttpRequest parse_request(const char *request);

void print_info(HttpRequest request);

#endif
