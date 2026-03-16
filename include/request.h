#ifndef __REQUEST_H__
#define __REQUEST_H__
#include "csapp.h"

typedef enum {
    GET = 0,
    PUT = 1,
    LS = 2,
    RM = 3
} typereq_t;

typedef struct {
    typereq_t typereq;
    char path[MAXLINE];
} request_t;
#endif