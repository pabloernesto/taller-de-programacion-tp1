#define _POSIX_C_SOURCE 201709L

#include "client.h"
#include "help.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "socket.h"

#include "courier.h"
#include <stdio.h>
#include <stdlib.h>

static int createConnection(char *node, char *service);
static void clientLoop(int connection);

void clientRoutine(int argc, char **argv) {
    if ((argc < 4) || (argc > 5)) { printHelp(); exit(1); }

    if (argv[4] != NULL) stdin = freopen(argv[4], "r", stdin);
    if (stdin == NULL) { perror("Could not open file"); exit(1); }

    int connection = createConnection(argv[2], argv[3]);

    clientLoop(connection);

    Socket_shutdown(.fd=connection);
    Socket_close(.fd=connection);
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

static void clientLoop(int connection) {
    Courier *courier = Courier_new(connection);
    do {
        struct command_s command = Courier_readCommand(courier);

        if (command.opcode == 0) {
            break;
        } else if (command.opcode == -1) {
            fprintf(stderr, "Error reading command. Exiting.");
            exit(1);
        }

        Courier_sendCommand(courier, command);
        if (command.opcode == 5) {
            struct response_s response = Courier_recvResponse(courier);
            printf("%s", response.data);
            Courier_destroyResponse(response);
        }

        Courier_destroyCommand(command);
    } while (1);

    Courier_destroy(courier);
}
