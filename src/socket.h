/* Clean wrappers around the Linux socket interface.
 *
 * Prerrequisites. (order is important)
 *     #define _POSIX_C_SOURCE >= 201112L
 *     #include <sys/types.h>
 *     #include <sys/socket.h>
 *     #include <netdb.h>
 *
 * The wrapper functions add only syntactic sugar (and the documentation is
 * consecuently limited to a catalogue of defaults) unless explicity noted.
 *
 * To override a macro's defaults, add the following as a parameter to the call:
 *     .parameterName=parameterValue
 * For example, when creating a socket with raw network access, instead of the
 * default TCP stream, call Socket_socket(.type=SOCK_RAW) */

/* Developer's note: An interface quirk.
 *
 * Every function in this header is actually a macro wrapping a call to a
 * function taking a struct. Even the functions that take a single, mandatory
 * argument, like Socket_close. This is in part a forced call.
 *
 * The c99 standard specifies that the macro(arg1, arg2, ...) form of
 * a variadic macro (a macro accepting a variable number of arguments)
 * must receive at least one argument to stand for the ... Which means that
 * functions may not receive some mandatory arguments and some optional
 * arguments without recurring the variadic function mechanism (a mechanism
 * distinct from the variadic macro that has no provision for type safety).
 *
 * Once decided that type safety and optional arguments would be at the core of
 * this interface, it was foregone conclusion that mixed-argument functions
 * would take their mandatory functions with the same optional argument mechanic
 * rather than forgoing optional arguments entirely. However, this leaves the
 * functions taking a mix of mandatory and optional arguments implemented with
 * a fully-optional-looking call syntax, and the functions that take only
 * mandatory arguments using a traditional call syntax. This is a confusing
 * mess, and in the interest of user sanity, the traditional syntax was avoided
 * entirely, resulting in the current state of affairs.
 */

#ifndef SOCKET_H
#define SOCKET_H

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
 * fd is a mandatory argument, and must be a valid file descriptor.
 *
 * name         default
 * ====         =======
 * fd           -1 (ERROR)
 * how          SHUT_RDWR
 */
#define Socket_shutdown(...) _shutdown((struct _shutdown_s){ .fd=-1, \
    .how=SHUT_RDWR, __VA_ARGS__ })

/* Close a socket.
 *
 * fd is a mandatory argument, and must be a valid file descriptor.
 * On success, zero is returned. On error, -1 is returned, and errno
 * is set appropriately. */
#define Socket_close(...) _close((struct _close_s){ .fd=-1, __VA_ARGS__ })

/* Initiates a connection on a socket.
 *
 * If the connection or binding succeeds, zero is returned. On error,
 * -1 is returned, and errno is set appropriately.
 *
 * fd is a mandatory argument, and must be a valid file descriptor.
 *
 * *addr is a struct addrinfo, obtained by a call to Socket_getaddrinfo,
 * and representing the address of the remote endpoint of the communication.
 *
 * addr MUST NOT be NULL. If that is the case, behaviour is undefined. */
#define Socket_connect(...) _connect(struct _connect_s){ .fd=-1, __VA_ARGS__ });

/* Binds a socket to the specified address.
 *
 * *addr is a struct addrinfo, obtained by a call to Socket_getaddrinfo,
 * and representing the address of the remote endpoint of the communication.
 *
 * fd is a mandatory argument, and must be a valid file descriptor.
 *
 * On success, zero is returned. On error, -1 is returned, and errno
 * is set appropriately. */
#define Socket_bind(...) _bind((struct _bind_s){ .fd=-1, .addr=NULL,\
    __VA_ARGS__})

/* Mark as a socket that will be used to accept incoming connection requests
 * using accept.
 *
 * On success, zero is returned. On error, -1 is returned, and errno
 * is set appropriately.
 *
 * fd is a mandatory argument, and must be a valid file descriptor.
 *
 * name         default
 * ====         =======
 * fd           -1 (ERROR)
 * backlog      10
 */
#define Socket_listen(...) _listen((struct _listen_s){ .fd=-1, \
    .backlog=10, __VA_ARGS__ })

/* Accept a connection on a socket.
 *
 * This function returns a accept_s structure, which is defined below.
 *
 * On success, the return structure's error element will be 0. On error,
 * the return structure's error element will be -1, and errno will be set
 * appropriately.
 *
 * fd is a mandatory argument, and must be a valid file descriptor.
 *
 * On success, fd will contain the file descriptor of the newly created socket,
 * addr will contain the peer's address, and addrlen will contain the length
 * of that address.
 *
 * As inputs, error, addr, and addrlen are ignored.
 */
#define Socket_accept(...) _accept((struct accept_s){ .fd=-1, __VA_ARGS__})
struct accept_s {
    int error, fd;
    struct sockaddr addr;
    socklen_t addrlen;
};

/* Performs network address and service translation.
 *
 * On success, 0 is returned. On failure, an error code is returned.
 * For explanations of the different error codes, check the getaddrinfo
 * documentation.
 *
 * res is a mandatory argument. *res must be an allocated pointer
 * ie: res may not be NULL.
 *
 * On success, *res will point to an in memory linked list of network addresses.
 * Call Socket_freeaddrinfo(*res) to deallocate this space.
 *
 * By default, Socket_getaddrinfo will set res to the local machine, port 8080.
 * This is adequate for binding a listening port.
 *
 * name         default
 * ====         =======
 * res          NULL (ERROR)
 * node         NULL
 * service      8080
 * ai_flags     AI_PASSIVE
 * ai_family    AF_INET
 * ai_socktype  SOCK_STREAM
 *
 * Usage example:
 *     struct addrinfo *res;
 *     int errcode = Socket_getaddrinfo(.res=res);
 *     if (errcode) return Socket_gai_strerror(errcode);
 *
 *     // Use *res, or *(res->ai_next), or *(res->ai_next->ai_next)...
 *
 *     Socket_freeaddrinfo(res);
 */
#define Socket_getaddrinfo(...) _getaddrinfo((struct _getaddinfo_s){ .res=NULL,\
    .service="8080", .ai_flags=AI_PASSIVE, .ai_family=AF_INET,                 \
    .ai_socktype=SOCK_STREAM, __VA_ARGS__})

/* Frees the list allocated by Socket_getaddrinfo. See usage example in the
 * documentation for Socket_getaddrinfo. */
#define Socket_freeaddrinfo(...) _freeaddrinfo((struct _freeaddrinfo_s){\
    .res=NULL, __VA_ARGS__ })

#define Socket_gai_strerror(...) _gai_strerror((struct _gai_strerror_s){\
    .errcode=0, __VA_ARGS__ })

/******************************************************************************/
/* Sending and receiving with sockets. */

/* Sends a message on a socket.
 *
 * On success, the number of characters written is returned. If this number is
 * less than len, the socket was shut down while transmitting. On error,
 * -1 is returned, and errno is set appropriately.
 *
 * fd is a mandatory argument, and must be a valid file descriptor.
 *
 * buf should point to an allocated array of char.
 * len must not exeed the number of elements of buf if buf is not NULL.
 * If len is 0, nothing is sent and buf is ignored.
 *
 * If Socket_send is used on a connection-mode (SOCK_STREAM, SOCK_SEQPACKET)
 * socket, the argument dest is ignored (and the error EISCONN may be returned
 * when it is not NULL), and the error ENOTCONN is returned when the socket
 * was not actually connected. Otherwise, the address of the target is given
 * by addr.
 *
 * name         default
 * ====         =======
 * fd           -1 (ERROR)
 * buf          NULL
 * len          0
 * flags        0
 * dest         NULL
 */
#define Socket_send(...) _send((struct _send_s){ .fd=-1, .buf=NULL, .len=0,\
    .flags=0, .dest=NULL, __VA_ARGS__ })

#define Socket_recv(...) _recv((struct _recv_s){ .fd=-1, .buf=NULL, .len=0,\
    .flags=0, __VA_ARGS__ })

/******************************************************************************/
/* For macro use only. It is possible, but adviced against, to manually handle
 * these functions and structures. */

struct _socket_s { int domain, type, protocol; };
int _socket(struct _socket_s in);

struct _shutdown_s { int fd, how; };
int _shutdown(struct _shutdown_s in);

struct _close_s { int fd; };
int _close(struct _close_s in);

struct _listen_s { int fd, backlog; };
int _listen(struct _listen_s in);

struct accept_s _accept(struct accept_s in);

struct _connect_s { int fd; struct addrinfo *addr; };
int _connect(struct _connect_s in);

struct _bind_s { int fd; struct addrinfo *addr; };
int _bind(struct _bind_s in);

struct _getaddinfo_s {struct addrinfo **res; char *node; char *service;
    int ai_flags, ai_family, ai_socktype, ai_protocol; };
int _getaddrinfo(struct _getaddinfo_s in);

struct _freeaddrinfo_s {struct addrinfo *res; };
void _freeaddrinfo(struct _freeaddrinfo_s in);

struct _gai_strerror_s { int errcode; };
const char *_gai_strerror(struct _gai_strerror_s in);

struct _send_s { int fd, flags, len; void *buf; struct addrinfo *dest; };
int _send(struct _send_s in);

struct _recv_s { int fd, len, flags; void *buf; };
int _recv(struct _recv_s in);

#endif
