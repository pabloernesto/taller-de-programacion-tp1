#define _POSIX_C_SOURCE 201709L

#include "server.h"
#include "help.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "socket.h"

#include "rope.h"
#include "courier.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

static int createListeningSocket(char *port);
static void serverLoop(Courier *courier, Rope *rope);

void serverRoutine(int argc, char **argv) {
    if (argc > 3) { printHelp(); exit(1); }
    int listening = createListeningSocket(argv[2] ? argv[2] : "8080");

    while (1) {
        struct accept_s ret = Socket_accept(.fd=listening);
        if (ret.error != 0) { perror("Accept failed"); break; }

        int connection = ret.fd;
        Courier *courier = Courier_new(connection);
        Rope *rope = Rope_new();

        serverLoop(courier, rope);

        Rope_destroy(rope);
        Courier_destroy(courier);
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

static void serverLoop(Courier *courier, Rope *rope) {
    do {
        struct command_s command = Courier_recvCommand(courier);

        if (command.opcode == 1) {
            Rope_insert(rope, command.u.i.pos, command.u.i.data);
        } else if (command.opcode == 2) {
            Rope_delete(rope, command.u.d.from, command.u.d.to);
        } else if (command.opcode == 3) {
            Rope_insert(rope, command.u.s.pos, " ");
        } else if (command.opcode == 4) {
            Rope_insert(rope, command.u.n.pos, "\n");
        } else if (command.opcode == 5) {
            char *s = Rope_toString(rope);
            struct response_s r = { .len=strlen(s), .data = s };
            Courier_sendResponse(courier, r);
            Courier_destroyResponse(r);
        } else {
            fprintf(stderr, "Unrecognized opcode: %d\n", command.opcode);
            Courier_destroyCommand(command);
            break;
        }

        Courier_destroyCommand(command);
    } while (1);
}
