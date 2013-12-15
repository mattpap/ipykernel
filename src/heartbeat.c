#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <zmq.h>

#include "sockets.h"

void* heartbeat(void* data) {
    Sockets* sockets = (Sockets*)data;
    zmq_proxy(sockets->heartbeat, sockets->heartbeat, NULL);
    pthread_exit(NULL);
    return NULL;
}

void init_heartbeat(Sockets* sockets) {
    pthread_t thread;

    if (pthread_create(&thread, NULL, heartbeat, (void*)sockets)) {
        fprintf(stderr, "error: unable to create heartbeat thread");
        exit(1);
    }
}
