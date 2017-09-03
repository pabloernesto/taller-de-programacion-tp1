#include "socket.h"
#include <unistd.h>
#include <assert.h>

int _socket(struct _socket_s in) {
    return socket(in.domain, in.type, in.protocol);
}

int _shutdown(int fd, struct _shutdown_s in) {
    return shutdown(fd, in.how);
}

int Socket_close(int fd) {
    return close(fd);
}

int Socket_connect(int fd, struct addrinfo *addr) {
    assert(addr != NULL);
    return connect(fd, addr->ai_addr, addr->ai_addrlen);
}

int Socket_bind(int fd, struct addrinfo *addr) {
    return bind(fd, addr->ai_addr, addr->ai_addrlen);
}

int _listen(int fd, struct _listen_s in) {
    return listen(fd, in.backlog);
}

int _getaddrinfo(struct addrinfo **res, struct _getaddinfo_s in) {
    struct addrinfo hints = (struct addrinfo) {
        .ai_flags=in.ai_flags,
        .ai_family=in.ai_family,
        .ai_socktype=in.ai_socktype,
        .ai_protocol=in.ai_protocol
    };

    return getaddrinfo(in.node, in.service, &hints, res);
}

void Socket_freeaddrinfo(struct addrinfo *res) {
    freeaddrinfo(res);
}

const char *Socket_gai_strerror(int errcode) {
    return gai_strerror(errcode);
}

int _send(int fd, const char *buf, int len, struct _send_s in) {
    do {
        int n = sendto(fd, buf, len, in.flags, in.dest.ai_addr,
                in.dest.ai_addrlen);
        if (n == -1) return -1;
        buf += n;
        len -= n;
    } while (len > 0);
    return 0;
}
