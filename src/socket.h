/* Clean wrappers around the Linux socket interface.
 *
 * The wrapper functions add only syntactic sugar (and the documentation is
 * consecuently limited to a catalogue of defaults) unless explicity noted.
 *
 * To override a macro's defaults, add the following as a parameter to the call:
 *     .parameterName=parameterValue
 * For example, when creating a socket with raw network access, instead of the
 * default TCP stream, call Socket_socket(.type=SOCK_RAW) */

#ifndef SOCKET_H
#define SOCKET_H

#define _POSIX_C_SOURCE 201709L

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef struct addrinfo addrinfo_s;

/******************************************************************************/
/* Opening and closing sockets. */

/* Wrapper around socket function.
 *
 * On success, a file descriptor for the new socket is returned. On error,
 * -1 is returned, and errno is set appropriately.
 *
 * name         default
 * ====         =======
 * domain       AF_INET
 * type         SOCK_STREAM
 * protocol     0
 */
#define Socket_socket(...) _socket((struct _socket_s){.domain=AF_INET, \
    .type=SOCK_STREAM, .protocol=0, __VA_ARGS__})

/* Shut down a connection partially or wholly.
 *
 * On success, zero is returned. On error, -1 is returned, and errno
 * is set appropriately.
 *
 * name         default
 * ====         =======
 * how          SHUT_RDWR
 */
#define Socket_shutdown(fd, ...) _shutdown(fd, (struct _shutdown_s){ \
    .how=SHUT_RDWR, __VA_ARGS__ }

int Socket_close(int fd);

/* Initiates a connection on a socket.
 *
 * If the connection or binding succeeds, zero is returned. On error,
 * -1 is returned, and errno is set appropriately.
 *
 * *addr is a struct addrinfo, obtained by a call to Socket_getaddrinfo,
 * and representing the address of the remote endpoint of the communication.
 *
 * addr MUST NOT be NULL. If that is the case, behaviour is undefined. */
int Socket_connect(int fd, addrinfo_s *addr);

/* Binds a socket to the specified address.
 *
 * *addr is a struct addrinfo, obtained by a call to Socket_getaddrinfo,
 * and representing the address of the remote endpoint of the communication.
 *
 * On success, zero is returned. On error, -1 is returned, and errno
 * is set appropriately. */
int Socket_bind(int fd, addrinfo_s *addr);

/* Listen for connections on a socket.
 *
 * On success, zero is returned. On error, -1 is returned, and errno
 * is set appropriately.
 *
 * name         default
 * ====         =======
 * backlog      10
 */
#define Socket_listen(fd, ...) _listen(fd, (struct _listen_s){ .backlog=10, \
    __VA_ARGS__ }

/* Performs network address and service translation.
 *
 * On success, 0 is returned. On failure, an error code is returned.
 * For explanations of the different error codes, check the getaddrinfo
 * documentation.
 *
 * *res must be an allocated pointer—ie: res may not be NULL.
 * On success, *res will point to an in memory linked list of network addresses.
 * Call Socket_freeaddrinfo(*res) to deallocate this space.
 *
 * By default, Socket_getaddrinfo will set res to the local machine, port 8080.
 * This is adequate for binding a listening port.
 *
 * name         default
 * ====         =======
 * node         NULL
 * service      8080
 * ai_flags     AI_PASSIVE
 * ai_family    AF_INET
 * ai_socktype  SOCK_STREAM
 *
 * Usage example:
 *     struct addrinfo *res;
 *     int errcode = Socket_getaddrinfo(res);
 *     if (errcode) return Socket_gai_strerror(errcode);
 *
 *     // Use *res, or *(res->ai_next), or *(res->ai_next->ai_next)...
 *
 *     Socket_freeaddrinfo(res);
 */
#define Socket_getaddrinfo(res, ...) _getaddrinfo(res, (struct _getaddinfo_s){ \
    .service="8080", .ai_flags=AI_PASSIVE, .ai_family=AF_INET,                 \
    ai_socktype=SOCK_STREAM, __VA_ARGS__})

/* Frees the list allocated by Socket_getaddrinfo. See usage example in the
 * documentation for Socket_getaddrinfo. */
void Socket_freeaddrinfo(xxx *res);

const char *Socket_gai_strerror(int errcode);

/******************************************************************************/
/* For macro use only. It is possible, but adviced against, to manually handle
 * these functions and structures. */

struct _socket_s { int domain, type, protocol; };
int _socket(struct _socket_s in);

struct _shutdown_s { int how; };
int _shutdown(int fd, struct _shutdown_s in);

struct _listen_s { int backlog; };
int _listen(int fd, struct _listen_s in);

struct _getaddinfo_s { const char *node; const char *service;
    int ai_flags, ai_family, ai_socktype, ai_protocol; };
int _getaddrinfo(addrinfo_s **res, struct _getaddinfo_s in);

#endif
