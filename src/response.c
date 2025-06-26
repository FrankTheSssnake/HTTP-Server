#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>

#include "../include/response.h"
#include "../include/request.h"
#include "../include/router.h"


HttpResponse *init_response() {
    HttpResponse *response = malloc(sizeof(HttpResponse));
    if (!response) {
        fprintf(stderr, "[ERROR] Failed to allocate memory for Httpresponse->\n");
        return NULL;
    }

    response->header.status_code = 200;
    response->header.content_length = 0;
    response->body = NULL;
    response->raw = NULL;

    return response;
}


const char *get_status_msg(uint16_t status_code) {
    switch (status_code) {
        case 200: return "OK";
        case 400: return "Bad Request";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 422: return "Unprocessable Content";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 503: return "Service Unavailable";
        default:  return "Unknown";
    }
}


void form_body(const char *body, HttpResponse *response) {
    response->body = malloc(strlen(body) + 1);

    if (!(response->body)) {
        fprintf(stderr, "[ERROR] Response body could not be formed.\n");
        response->header.status_code = 500;
        return;
    }

    if (response->header.content_length == 0) {
        response->header.content_length = strlen(body);
    }

    strcpy(response->body, body);
}


void form_header(HttpResponse *response) {
    if (!(response->body)) return;

    response->header.status_msg = get_status_msg(response->header.status_code);

    const char *header_format = "HTTP/1.1 %d %s\r\nContent-Type: text/plain\r\nContent-Length: %lu\r\nConnection: close\r\n\r\n";

    int needed = snprintf(NULL, 0, header_format,
            response->header.status_code, response->header.status_msg, response->header.content_length
    );

    response->header.raw_header = malloc(needed + 1);

    snprintf(response->header.raw_header, needed + 1, header_format,
            response->header.status_code, response->header.status_msg, response->header.content_length
    );
}


void form_raw(HttpResponse *response) {
    if (!response->header.raw_header || !response->body) {
        fprintf(stderr, "[ERROR] Header or body not properly initialized.\n");
        return;
    }
    
    size_t total_len = strlen(response->header.raw_header) + strlen(response->body) + 1;
    response->raw = malloc(total_len);
    
    if (!response->raw) {
        fprintf(stderr, "[ERROR] Failed to allocate memory for raw response.\n");
        return;
    }

    strcpy(response->raw, response->header.raw_header);
    strcat(response->raw, response->body);
}


HttpResponse *form_response(HttpRequest *request) {
    HttpResponse *response = init_response();
    char *body = NULL;

    switch (request->method) {
        case GET:
        case HEAD:
            body = route_request(request, response);
            break;
        case INVALID:
            response->header.status_code = 400;
            body = "";
            break;
        default:
            response->header.status_code = 501;
            body = "";
            break;
    }

    form_body(body, response);
    form_header(response);

    return response;
}


void free_response(HttpResponse *response) {
    if (!response) return;

    if (response->header.raw_header) {
        free(response->header.raw_header);
        response->header.raw_header = NULL;
    }

    if (response->body) {
        free(response->body);
        response->body = NULL;
    }

    if (response->raw) {
        free(response->raw);
        response->raw = NULL;
    }

    free(response);
}



int send_response(int client_fd, HttpRequest request) {
    HttpResponse *response = form_response(&request);

    form_raw(response);
    
    if (!response->raw) {
        fprintf(stderr, "[ERROR] Failed to form raw response.\n");
        free_response(response);
        return -1;
    }
 
    if (send(client_fd, response->raw, strlen(response->raw), 0) < 0) {
        perror("[FAIL] `send`: ");
        free_response(response);
        return -1;
    }

    free_response(response);

    return 0;
}
