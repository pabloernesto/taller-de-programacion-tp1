#define _POSIX_C_SOURCE 201709L

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdio.h>
#include "socket.h"

int main(int argc, char **argv) {
    int fd = Socket_socket();
    if (fd == -1) { perror("Could not open socket.\n"); return 1; }

    struct addrinfo *res;
    int errcode = Socket_getaddrinfo(.res=&res);
    if (errcode) {
        perror("Could not get local address: ");
        perror(Socket_gai_strerror(.errcode=errcode));
        return 1;
    }

    if (Socket_bind(.fd=fd, .addr=res)) { perror("Could not bind."); return 1; }
    Socket_freeaddrinfo(.res=res);

    if (Socket_listen(.fd=fd)) { perror("Could not listen."); return 1; }

    while (1) {
        struct accept_s ret = Socket_accept(.fd=fd);
        if (ret.error != 0) break;

        Socket_shutdown(.fd=ret.fd);
        Socket_close(.fd=ret.fd);
    }

    Socket_close(.fd=fd);
}
