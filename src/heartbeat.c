#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <zmq.h>

#include "globals.h"

void* heartbeat(void* data) {
    zmq_device(ZMQ_STREAMER, sockets.heartbeat, sockets.heartbeat);
    pthread_exit(NULL);
    return NULL;
}

void init_heartbeat() {
    pthread_t thread;

    if (pthread_create(&thread, NULL, heartbeat, NULL)) {
        fprintf(stderr, "error: unable to create heartbeat thread\n");
        exit(1);
    }
}
