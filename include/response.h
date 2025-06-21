#ifndef RESPONSE_H
#define RESPONSE_H

#include "request.h"

char *form_header(HttpRequest request);
char *form_body(HttpRequest request);
char *form_response(HttpRequest request);
int send_response(int sock_fd, HttpRequest request);
void print_info(HttpRequest request);

#endif
