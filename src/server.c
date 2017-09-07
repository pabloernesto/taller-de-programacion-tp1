#define _POSIX_C_SOURCE 201709L

#include "server.h"
#include "help.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "socket.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

static int createListeningSocket(char *port);
static void echoSocket(const int fd);

static void handlePrint(void);

void serverRoutine(int argc, char **argv) {
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
    int opcode;
    do {
        int n = Socket_recv(.fd=connection, .buf=&opcode, .len=4);
        if (n == -1) {
            perror("echoSocket: socket failure while receiving opcode");
            return;
        } else if (n == 0) {
            printf("Connection shut down\n");
            return;
        } else if (n < 4) {
            fprintf(stderr, "echoSocket: "
                            "socket shut down while receiving opcode.");
            return;
        }

        opcode = ntohl(opcode);
        if (opcode == 5) {
            handlePrint();
        } else {
            fprintf(stderr, "Unrecognized opcode: %d\n", opcode);
            return;
        }
    } while (1);
}

static void handlePrint(void) {
    printf("print\n");
}
