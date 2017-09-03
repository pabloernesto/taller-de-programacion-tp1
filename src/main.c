#include <stdio.h>
#include "socket.h"

int main(int argc, char **argv) {
    int s = Socket_socket();

    if (s == -1) printf("-1\n");
    else printf("%d\n", s);

    Socket_close(s);
}
