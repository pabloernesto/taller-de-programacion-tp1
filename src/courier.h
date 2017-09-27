#include "socket.h"

#ifndef COURIER_H
#define COURIER_H

/* Please be adviced: command_s and response_s also have destructor functions
 * associated. */

struct insert_command_s { int pos; short int len; char *data; };
struct delete_command_s { int from; int to; };
struct space_command_s { int pos; };
struct newline_command_s { int pos; };

enum opcodes {COURIER_INSERT=1, COURIER_DELETE, COURIER_SPACE,
                COURIER_NEWLINE, COURIER_PRINT};

struct command_s {
    int opcode;
    union _command_container {
        struct insert_command_s i;
        struct delete_command_s d;
        struct space_command_s s;
        struct newline_command_s n;
    } u;
};

struct response_s { int len; char *data; };

typedef struct Courier Courier;

/******************************************************************************/
/* Creator and destructor. */

Courier *Courier_new(socket_t *socket);

/* Will not close the socket. */
void Courier_destroy(Courier *self);

void Courier_destroyCommand(struct command_s self);

void Courier_destroyResponse(struct response_s self);

/******************************************************************************/
/* Operations. */

/* Reads a command from stdin.
 *
 * On error, opcode will be -1. If stdin has reached EOF, and no opcode has
 * been read yet, opcode will be 0. */
struct command_s Courier_readCommand(Courier *self);

/* Reads a command from the network socket.
 *
 * On error, opcode will be -1. If socket has shut down, and no opcode has
 * been read yet, opcode will be 0. */
struct command_s Courier_recvCommand(Courier *self);

/* Sends a command through the network socket.
 *
 * On success, 0 is returned. On error, -1 is returned */
int Courier_sendCommand(Courier *self, struct command_s command);

/* Reads a response from the network socket.
 *
 * On error, len will be -1. */
struct response_s Courier_recvResponse(Courier *self);

/* Sends a response through the network socket.
 *
 * On success, 0 is returned. On error, -1 is returned */
int Courier_sendResponse(Courier *self, struct response_s r);

#endif
