#ifndef ROUTER_H
#define ROUTER_H

#include <stdlib.h>

#include "../include/response.h"
#include "../include/request.h"

typedef char *(*RouteHandler)(HttpRequest *, HttpResponse *);

typedef struct {
    METHOD method;
    RouteHandler handler;
} Route;

char *route_request(HttpRequest *request, HttpResponse *response);

char *get_or_head(HttpRequest *request, HttpResponse *response);

char *invalid(HttpRequest *request, HttpResponse *response);

char *not_supported(HttpRequest *request, HttpResponse *response);
#endif
