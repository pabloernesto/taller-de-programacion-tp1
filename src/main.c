#define _POSIX_C_SOURCE 201709L

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "socket.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static void printHelp();
static void serve(int argc, char **argv);
static int createListeningSocket(char *port);
static void echoSocket(const int fd);

int main(int argc, char **argv) {
    if (argc < 2) { printHelp(); exit(0); }

    if (strcmp(argv[1], "server") == 0) serve(argc, argv);
    //if (strcmp(argv[1], "client") == 0) connect(argc, argv);
}

static void printHelp() {
    printf("./tp server [<port>]\n"
           "./tp client <host> <port> [<inputfile>]\n");
}

static void serve(int argc, char **argv) {
    if (argc > 3) { printHelp(); exit(1); }
    int listening = createListeningSocket(argv[2] ? argv[2] : "8080");

    while (1) {
        struct accept_s ret = Socket_accept(.fd=listening);
        if (ret.error != 0) { perror("Accept failed"); break; }

        int connection = ret.fd;
        echoSocket(connection);

        Socket_shutdown(.fd=connection);
        Socket_close(.fd=connection);
    }

    Socket_close(.fd=listening);
}

static int createListeningSocket(char *port) {
    port = port ? port : "8080";

    int fd = Socket_socket();
    if (fd == -1) { perror("Could not open socket"); exit(1); }

    struct addrinfo *res;
    int errcode = Socket_getaddrinfo(.res=&res, .service=port);
    if (errcode) {
        perror("Could not get local address");
        exit(1);
    }

    if (Socket_bind(.fd=fd, .addr=res)) { perror("Could not bind"); exit(1); }
    Socket_freeaddrinfo(.res=res);

    if (Socket_listen(.fd=fd)) { perror("Could not listen"); exit(1); }

    return fd;
}

static void echoSocket(const int connection) {
    #define SIZE 256
    char buf[SIZE];
    int n;
    do {
        n = Socket_recv(.fd=connection, .buf=buf, .len=SIZE);
        if (write(1, buf, n) == -1) { perror("Failed to write"); exit(1); }
    } while (n == SIZE);
    #undef SIZE
}
