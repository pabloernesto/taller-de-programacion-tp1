#define _POSIX_C_SOURCE 201709L

#include "client.h"
#include "help.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "socket.h"

#include "courier.h"
#include <stdio.h>

static void clientLoop(socket_t *sock);

void clientRoutine(int argc, char **argv) {
    if ((argc < 4) || (argc > 5)) { printHelp(); return; }

    if (argv[4] != NULL) stdin = freopen(argv[4], "r", stdin);
    if (stdin == NULL) { perror("Could not open file"); return; }

    socket_t sock;
    short portNumber;
    sscanf(argv[3], "%hd", &portNumber);
    if (socket_create(&sock) || socket_connect(&sock, argv[2], portNumber))
        goto closeInput;

    clientLoop(&sock);

    socket_destroy(&sock);
closeInput:
    fclose(stdin);
}

static void clientLoop(socket_t *sock) {
    Courier *courier = Courier_new(sock);
    do {
        struct command_s command = Courier_readCommand(courier);

        if (command.opcode < 1) break;

        Courier_sendCommand(courier, command);
        if (command.opcode == COURIER_PRINT) {
            struct response_s response = Courier_recvResponse(courier);
            printf("%s", response.data);
            Courier_destroyResponse(response);
        }

        Courier_destroyCommand(command);
    } while (1);

    Courier_destroy(courier);
}
