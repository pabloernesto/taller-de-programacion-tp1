#define _POSIX_C_SOURCE 201709L

#include "client.h"
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

static int createConnection(char *node, char *service);
static void clientLoop(int connection);

static void sendPrint(int connection);
static void sendSpace(int connection);

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
    const int BUFSIZE = 32;
    char s[BUFSIZE];
    while (scanf("%s31", s) != EOF) {
        if (strcmp("print", s) == 0) {
            sendPrint(connection);
        } else if (strcmp("space", s) == 0) {
            sendSpace(connection);
        } else {
            fprintf(stderr, "Unknown command: %s", s);
            exit(1);
        }
    }
}

static void sendPrint(int connection) {
    int opcode = htonl(5);
    int n = Socket_send(.fd=connection, .buf=&opcode, .len=4);
    if (n < 4) {
        fprintf(stderr, "sendPrint: Socket_send failed: ");
        if (n < 0)
            perror(NULL);
        else
            fprintf(stderr, "connection shutdown.\n");
        exit(1);
    }
}

static void sendSpace(int connection) {
    /* Preparing package. */
    int package[2];
    package[0] = htonl(3);
    if (scanf("%d", package + 1) < 1) {
        fprintf(stderr, "sendPrint: could not read position");
        exit(1);
    }

    /* Sending. */
    int n = Socket_send(.fd=connection, .buf=package, .len=8);
    if (n < 8) {
        fprintf(stderr, "sendPrint: package send failed");

        if (n >= 0) {
            fprintf(stderr, ", connection shutdown.\n");
        } else {
            fprintf(stderr, ":");
            perror(NULL);
        }
        exit(1);
    }
}
