#define _POSIX_C_SOURCE 201709L

#include "courier.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void readPrint(struct command_s *in);
static void readSpace(struct command_s *in);
static void readNewline(struct command_s *in);
static void readInsert(struct command_s *in);
static void readDelete(struct command_s *in);

static int sendLong(Courier *self, int l);
static int sendShort(Courier *self, short int s);
static int sendString(Courier *self, short int len, char *buf);
static int sendLongString(Courier *self, int len, char *buf);

static int recvLong(Courier *self, int *l);
static int recvShort(Courier *self, short int *s);
static int recvString(Courier *self, short int *len, char **buf);
static int recvLongString(Courier *self, int *len, char **buf);

struct Courier { socket_t *socket; };

Courier *Courier_new(socket_t *socket) {
    if (!socket || (socket->socket < 0)) return NULL;

    Courier *self = malloc(sizeof(Courier));
    if (!self) return NULL;

    *self = (Courier){ .socket=socket };
    return self;
}

void Courier_destroy(Courier *self) {
    free(self);
}

void Courier_destroyCommand(struct command_s self) {
    if ((self.opcode == 1) && (self.u.i.data))
        free(self.u.i.data);
}

void Courier_destroyResponse(struct response_s self) {
    if (self.data)
        free(self.data);
}

struct command_s Courier_readCommand(Courier *self) {
    struct command_s ret = { .opcode=0 };

    const int BUFSIZE = 32;
    char s[BUFSIZE];
    int n = scanf("%31s", s);

    if ((n <= 0) || (n == EOF)) {
        ret = (struct command_s){ .opcode=0 };
        return ret;
    }

    if (strcmp("print", s) == 0) {
        readPrint(&ret);
    } else if (strcmp("space", s) == 0) {
        readSpace(&ret);
    } else if (strcmp("newline", s) == 0) {
        readNewline(&ret);
    } else if (strcmp("insert", s) == 0) {
        readInsert(&ret);
    } else if (strcmp("delete", s) == 0) {
        readDelete(&ret);
    } else {
        fprintf(stderr, "Unknown command: %s", s);
        ret = (struct command_s){ .opcode=-1 };
    }
    return ret;
}

struct command_s Courier_recvCommand(Courier *self) {
    struct command_s command;

    if (recvLong(self, &(command.opcode)) == -1) {
        command = (struct command_s){ .opcode=0 };
        return command;
    }

    if (command.opcode == 1) {
        if ((-1 == recvLong(self, &(command.u.i.pos))) ||
                (-1 == recvString(self, &(command.u.i.len),
                                  &(command.u.i.data))))
            command = (struct command_s){ .opcode=-1 };
    } else if (command.opcode == 2) {
        if ((-1 == recvLong(self, &(command.u.d.from))) ||
                (-1 == recvLong(self, &(command.u.d.to))))
            command = (struct command_s){ .opcode=-1 };
    } else if (command.opcode == 3) {
        if (-1 == recvLong(self, &(command.u.s.pos)))
            command = (struct command_s){ .opcode=-1 };
    } else if (command.opcode == 4) {
        if (-1 == recvLong(self, &(command.u.n.pos)))
            command = (struct command_s){ .opcode=-1 };
    } else if (command.opcode == 5) {
    } else {
        fprintf(stderr, "Unrecoginzed opcode: %d\n", command.opcode);
        command = (struct command_s){ .opcode=-1 };
    }
    return command;
}

int Courier_sendCommand(Courier *self, struct command_s command) {
    if (command.opcode == 1) {
        if ((-1 == sendLong(self, command.opcode)) ||
                (-1 == sendLong(self, command.u.i.pos)) ||
                (-1 == sendString(self, command.u.i.len, command.u.i.data)))
            return -1;
    } else if (command.opcode == 2) {
        if ((-1 == sendLong(self, command.opcode)) ||
                (-1 == sendLong(self, command.u.d.from)) ||
                (-1 == sendLong(self, command.u.d.to)))
            return -1;
    } else if (command.opcode == 3) {
        if ((-1 == sendLong(self, command.opcode)) ||
                (-1 == sendLong(self, command.u.s.pos)))
            return -1;
    } else if (command.opcode == 4) {
        if ((-1 == sendLong(self, command.opcode)) ||
                (-1 == sendLong(self, command.u.n.pos)))
            return -1;
    } else if (command.opcode == 5) {
        if (-1 == sendLong(self, command.opcode)) return -1;
    }
    return 0;
}

struct response_s Courier_recvResponse(Courier *self) {
    struct response_s r;
    if (recvLongString(self, &(r.len), &(r.data)) == -1) {
        r = (struct response_s){ .len=-1 };
    }
    return r;
}

int Courier_sendResponse(Courier *self, struct response_s r) {
    if (sendLongString(self, r.len, r.data) == -1) return -1;
    return 0;
}

static void readPrint(struct command_s *in) {
    in->opcode = 5;
}

static void readSpace(struct command_s *in) {
    if (scanf("%d", &(in->u.s.pos)) == 1)
        in->opcode = 3;
    else
        in->opcode = -1;
}

static void readNewline(struct command_s *in) {
    if (scanf("%d", &(in->u.n.pos)) == 1)
        in->opcode = 4;
    else
        in->opcode = -1;
}

static void readInsert(struct command_s *in) {
    char *s = malloc(257);
    if (!s) return;

    if (scanf("%d %256s", &(in->u.i.pos), s) == 2) {
        in->u.i.len = (short int) strlen(s);
        in->u.i.data = s;
        in->opcode = 1;
    } else {
        free(s);
        in->opcode = -1;
    }
}

static void readDelete(struct command_s *in) {
    if (scanf("%d %d", &(in->u.d.from), &(in->u.d.to)) == 2)
        in->opcode = 2;
    else
        in->opcode = -1;
}

static int sendLong(Courier *self, int l) {
    l = htonl(l);
    return socket_send(self->socket, &l, 4);
}

static int sendShort(Courier *self, short int s) {
    s = htons(s);
    return socket_send(self->socket, &s, 2);
}

static int sendString(Courier *self, short int len, char *buf) {
    if (sendShort(self, len) == -1) return -1;
    return socket_send(self->socket, buf, len);
}

static int sendLongString(Courier *self, int len, char *buf) {
    if (sendLong(self, len) == -1) return -1;
    return socket_send(self->socket, buf, len);
}

static int recvLong(Courier *self, int *l) {
    if (socket_receive(self->socket, l, 4)) return -1;
    *l = ntohl(*l);
    return 0;
}

static int recvShort(Courier *self, short int *s) {
    if (socket_receive(self->socket, s, 2)) return -1;
    *s = ntohs(*s);
    return 0;
}

static int recvString(Courier *self, short int *len, char **buf) {
    if (recvShort(self, len) == -1) return -1;

    *buf = malloc(*len + 1);
    if (!buf) return -1;

    if (socket_receive(self->socket, *buf, *len)) {
        free(*buf);
        return -1;
    }

    (*buf)[*len] = '\0';
    return 0;
}

static int recvLongString(Courier *self, int *len, char **buf) {
    if (recvLong(self, len) == -1) return -1;

    *buf = malloc(*len + 1);
    if(!buf) return -1;

    if (socket_receive(self->socket, *buf, *len)) {
        free(*buf);
        return -1;
    }

    (*buf)[*len] = '\0';
    return 0;
}
