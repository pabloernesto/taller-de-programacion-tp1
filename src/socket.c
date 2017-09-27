#define _POSIX_C_SOURCE 201709L
#define _ISOC99_SOURCE //snprintf
#include "socket.h"

#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

#define SERVER_BACKLOG 10

static int _getaddrinfo(const char *host_name, unsigned short port,
                        struct addrinfo **out);

int socket_create(socket_t *self) {
    if (!self) return -2;
    *self = (socket_t){.socket=socket(AF_INET, SOCK_STREAM, 0)};
    if (self->socket == -1) return -1;
    return 0;
}

int socket_destroy(socket_t *self) {
    if ((!self) || (self->socket < 0)) return -2;
    socket_shutdown(self);
    return 0;
}

int socket_bind_and_listen(socket_t *self, unsigned short port) {
    if ((!self) || (self->socket < 0)) return -2;

    struct addrinfo *addr;
    if (_getaddrinfo(NULL, port, &addr)) return -1;

    struct addrinfo *rp;
    for (rp = addr; rp; rp = rp->ai_next)
        if (!bind(self->socket, rp->ai_addr, rp->ai_addrlen)) break;
    freeaddrinfo(addr);
    if (!rp) return -1;

    if (listen(self->socket, SERVER_BACKLOG)) return -1;
    return 0;
}

static int _getaddrinfo(const char *host_name, unsigned short port,
                        struct addrinfo **out) {
    // The maximum value of an unsigned short is 65535, five digits long.
    char portString[6];
    snprintf(portString, sizeof(portString), "%d", port);

    struct addrinfo hints = (struct addrinfo) {
        .ai_flags=AI_PASSIVE,
        .ai_family=AF_INET,
        .ai_socktype=SOCK_STREAM
    };

    if (getaddrinfo(host_name, portString, &hints, out)) return -1;
    return 0;
}

int socket_connect(socket_t *self, const char* host_name, unsigned short port) {
    if ((!self) || (self->socket < 0) || (!host_name)) return -2;

    struct addrinfo *addr;
    if (_getaddrinfo(host_name, port, &addr)) return -1;

    struct addrinfo *rp;
    for (rp = addr; rp; rp = rp->ai_next)
        if (!connect(self->socket, rp->ai_addr, rp->ai_addrlen)) break;
    freeaddrinfo(addr);
    if (!rp) return -1;

    return 0;
}

int socket_accept(socket_t *self, socket_t* accepted_socket) {
    if ((!self) || (self->socket < 0)) return -2;
    *accepted_socket = (socket_t){.socket=accept(self->socket, NULL, NULL)};
    if (accepted_socket->socket < 0) return -1;
    return 0;
}

int socket_send(socket_t *self, const void* buffer, size_t length) {
    const char *end = (char*)buffer + length;
    do {
        int n = send(self->socket, buffer, length, 0);
        if (n < 1) return -1;
        length -= n;
        buffer = (char*)buffer + n;
    } while (buffer != end);
    return 0;
}

int socket_receive(socket_t *self, void* buffer, size_t length) {
    if (length == 0) return 0;
    char *end = (char*)buffer + length;
    do {
        int n = recv(self->socket, buffer, length, 0);
        if (n < 1) return -1;
        length -= n;
        buffer = (char*)buffer + n;
    } while (buffer != end);
    return 0;
}

void socket_shutdown(socket_t *self) {
    shutdown(self->socket, SHUT_RDWR);
}
