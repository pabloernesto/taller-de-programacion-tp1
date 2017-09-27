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

static void serverLoop(Courier *courier);

void serverRoutine(int argc, char **argv) {
    if (argc > 3) { printHelp(); exit(1); }

    socket_t sock;
    if (socket_create(&sock)) return;

    short portNumber;
    sscanf(argv[2] ? argv[2] : "8080", "%hd", &portNumber);
    if (socket_bind_and_listen(&sock, portNumber)) goto outro;

    while (1) {
        socket_t incoming;
        if (socket_accept(&sock, &incoming)) continue;

        Courier *courier = Courier_new(&incoming);

        serverLoop(courier);

        Courier_destroy(courier);
        socket_destroy(&incoming);
    }

outro:
    socket_destroy(&sock);
}

static void serverLoop(Courier *courier) {
    Rope *rope = Rope_new();

    do {
        struct command_s command = Courier_recvCommand(courier);

        if (command.opcode == 1) {
            rope = Rope_insert(rope, command.u.i.pos, command.u.i.data);
        } else if (command.opcode == 2) {
            rope = Rope_delete(rope, command.u.d.from, command.u.d.to);
        } else if (command.opcode == 3) {
            rope = Rope_insert(rope, command.u.s.pos, " ");
        } else if (command.opcode == 4) {
            rope = Rope_insert(rope, command.u.n.pos, "\n");
        } else if (command.opcode == 5) {
            char *s = Rope_toString(rope);
            struct response_s r = { .len=strlen(s), .data=s };
            Courier_sendResponse(courier, r);
            Courier_destroyResponse(r);
        } else if (command.opcode == 0) {
            Courier_destroyCommand(command);
            break;
        } else {
            fprintf(stderr, "Unrecognized opcode: %d\n", command.opcode);
            Courier_destroyCommand(command);
            break;
        }

        Courier_destroyCommand(command);
    } while (1);

    Rope_destroy(rope);
}
