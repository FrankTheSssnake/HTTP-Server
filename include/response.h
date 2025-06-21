#ifndef RESPONSE_H
#define RESPONSE_H

char *form_header(char *request);
char *form_body(char *request);
char *form_response(char *request);
int send_response(int sock_fd, char *request);

#endif
