#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "profile.h"
#include "sockets.h"
#include "heartbeat.h"
#include "eventloop.h"

int main(int argc, char** argv) {
    Profile profile;
    Sockets sockets;

    init_profile(&profile, NULL);
    init_sockets(&sockets, &profile);

    init_heartbeat(&sockets);

    init_eventloop(sockets.requests);
    init_eventloop(sockets.control);

    pthread_exit(NULL);
    return 0;
}
