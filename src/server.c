#define _POSIX_C_SOURCE 201709L

#include "server.h"
#include "help.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "socket.h"

#include "courier.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

static int createListeningSocket(char *port);
static void serverLoop(const int fd);

void serverRoutine(int argc, char **argv) {
    if (argc > 3) { printHelp(); exit(1); }
    int listening = createListeningSocket(argv[2] ? argv[2] : "8080");

    while (1) {
        struct accept_s ret = Socket_accept(.fd=listening);
        if (ret.error != 0) { perror("Accept failed"); break; }

        int connection = ret.fd;
        serverLoop(connection);

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

static void serverLoop(const int connection) {
    Courier *courier = Courier_new(connection);
    do {
        struct command_s command = Courier_recvCommand(courier);

        if (command.opcode == 1) {
            printf("insert\n");
        } else if (command.opcode == 2) {
            printf("delete\n");
        } else if (command.opcode == 3) {
            printf("space\n");
        } else if (command.opcode == 4) {
            printf("newline\n");
        } else if (command.opcode == 5) {
            printf("print\n");
        } else {
            fprintf(stderr, "Unrecognized opcode: %d\n", command.opcode);
            break;
        }
        Courier_destroyCommand(command);
    } while (1);

    Courier_destroy(courier);
}
