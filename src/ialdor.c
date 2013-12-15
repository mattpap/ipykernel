#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "globals.h"
#include "util.h"
#include "options.h"
#include "profile.h"
#include "sockets.h"
#include "heartbeat.h"
#include "eventloop.h"

int main(int argc, char** argv) {
    init_options(&argc, &argv, &options);
    init_profile(&profile, options.profile);
    init_sockets(&sockets);

    init_heartbeat();

    init_eventloop(sockets.requests);
    init_eventloop(sockets.control);

    pthread_exit(NULL);
    return 0;
}
