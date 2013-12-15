#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <zmq.h>

void* eventloop(void* socket) {
    zmq_msg_t message;

    while (1) {
        zmq_msg_init(&message);
        zmq_msg_recv(&message, socket, 0);
        //  TODO
        printf("got message");
        zmq_msg_close(&message);

        if (!zmq_msg_more(&message))
            break;
    }

    pthread_exit(NULL);
    return NULL;
}

void init_eventloop(void* socket) {
    pthread_t thread;

    if (pthread_create(&thread, NULL, eventloop, socket)) {
        fprintf(stderr, "error: unable to create event loop thread\n");
        exit(1);
    }
}
