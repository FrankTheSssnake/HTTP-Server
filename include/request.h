#ifndef REQUEST_H
#define REQUEST_H

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
    int body_length;
    char body[1024];
} HttpRequest;


METHOD get_method(const char *method);

HttpRequest parse_request(const char *request);

#endif
