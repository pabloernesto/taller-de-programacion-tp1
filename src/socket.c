#define _POSIX_C_SOURCE 201709L

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "socket.h"
#include <unistd.h>
#include <assert.h>

int _socket(struct _socket_s in) {
    return socket(in.domain, in.type, in.protocol);
}

int _shutdown(struct _shutdown_s in) {
    return shutdown(in.fd, in.how);
}

int _close(struct _close_s in) {
    return close(in.fd);
}

int _connect(struct _connect_s in) {
    assert(in.addr != NULL);
    return connect(in.fd, in.addr->ai_addr, in.addr->ai_addrlen);
}

int _bind(struct _bind_s in) {
    struct sockaddr *addr = in.addr ? in.addr->ai_addr : NULL;
    socklen_t len = in.addr ? in.addr->ai_addrlen : 0;
    return bind(in.fd, addr, len);
}

int _listen(struct _listen_s in) {
    return listen(in.fd, in.backlog);
}

/* NOTE TO DEVELOPERS: len can be modified by the accept call (see accept(2))
 * and this function silently drops it's final value. This is an unresolved
 * bug. If you require this functionality you will have to work around it
 * by calling accept directly. */
struct accept_s _accept(struct accept_s in) {
    struct sockaddr addr;
    socklen_t len;
    int ret = accept(in.fd, &addr, &len);
    return (struct accept_s){
        .error = (ret ? -1 : 0),
        .fd = ret,
        .addr = addr,
        .addrlen = len
    };
}

int _getaddrinfo(struct _getaddinfo_s in) {
    struct addrinfo hints = (struct addrinfo) {
        .ai_flags=in.ai_flags,
        .ai_family=in.ai_family,
        .ai_socktype=in.ai_socktype,
        .ai_protocol=in.ai_protocol
    };

    return getaddrinfo(in.node, in.service, &hints, in.res);
}

void _freeaddrinfo(struct _freeaddrinfo_s in) {
    freeaddrinfo(in.res);
}

const char *_gai_strerror(struct _gai_strerror_s in) {
    return gai_strerror(in.errcode);
}

int _send(struct _send_s in) {
    char *p = in.buf;
    do {
        int n = sendto(in.fd, p, in.len, in.flags,
                in.dest ? in.dest->ai_addr : NULL,
                in.dest ? in.dest->ai_addrlen : 0);
        if (n == -1) return -1;
        p += n;
        in.len -= n;
    } while (in.len > 0);
    return 0;
}

int _recv(struct _recv_s in) {
    char *p = in.buf;
    do {
        int n = recv(in.fd, p, in.len, in.flags);
        if (n == -1) return -1;
        p += n;
        in.len -= n;
    } while (in.len > 0);
    return 0;
}
