#ifndef SERVER_H
#define SERVER_H

#include<stdio.h>
#include<stdlib.h>

#include<executor.h>

void serve(void (*connection_handler)(int fd));

#endif