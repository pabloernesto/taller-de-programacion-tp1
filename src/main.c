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
static void serverRoutine(int argc, char **argv);
static void clientRoutine(int argc, char **argv);

static void clientLoop(int connection, FILE *input);
static int createListeningSocket(char *port);
static int createConnection(char *node, char *service);
static void echoSocket(const int fd);

int main(int argc, char **argv) {
    if (argc < 2) { printHelp(); exit(0); }

    if (strcmp(argv[1], "server") == 0) serverRoutine(argc, argv);
    if (strcmp(argv[1], "client") == 0) clientRoutine(argc, argv);
}

static void printHelp() {
    printf("./tp server [<port>]\n"
           "./tp client <host> <port> [<inputfile>]\n");
}

static void serverRoutine(int argc, char **argv) {
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

static void clientRoutine(int argc, char **argv) {
    if ((argc < 4) || (argc > 5)) { printHelp(); exit(1); }

    FILE *input = argv[4] ? fopen(argv[4], "r") : stdin;
    if (input == NULL) { perror("Could not open file"); exit(1); }

    int connection = createConnection(argv[2], argv[3]);

    clientLoop(connection, input);

    Socket_shutdown(.fd=connection);
    Socket_close(.fd=connection);
    fclose(input);
}

static void clientLoop(int connection, FILE *input) {
    char *s;
    while (fscanf(input, "%ms", &s) != EOF) {
        Socket_send(.fd=connection, .buf=s, .len=strlen(s));
        free(s);
    }
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

static int createConnection(char *node, char *service) {
    struct addrinfo *address;
    int errcode = Socket_getaddrinfo(.res=&address, .node=node,
                                     .service=service);
    if (errcode) { perror("Could not get remote address"); exit(1);}

    int connection = Socket_socket();
    if (connection == -1) { perror("Could not open socket"); exit(1); }

    if (Socket_connect(.fd=connection, .addr=address) == -1) {
        perror("Could not connect");
        exit(1);
    }

    Socket_freeaddrinfo(.res=address);
    return connection;
}
