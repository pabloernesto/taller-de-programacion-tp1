#include "client.h"
#include "server.h"
#include "help.h"

#include <string.h>

int main(int argc, char **argv) {
    if (argc < 2) { printHelp(); return 0; }

    if (strcmp(argv[1], "server") == 0) serverRoutine(argc, argv);
    if (strcmp(argv[1], "client") == 0) clientRoutine(argc, argv);
}
