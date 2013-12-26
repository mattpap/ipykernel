#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <zmq.h>

#include "zeromq.h"

char* srecv(void* socket) {
    zmq_msg_t message;

    zmq_msg_init(&message);
    int rc = zmq_recv(socket, &message, 0);

    if (rc == -1) {
        fprintf(stderr, "error: failed to receive from ZMQ socket (errno=%d)\n", errno);
        exit(1);
    }

    int size = zmq_msg_size(&message);
    char* string = malloc(size+1);
    memcpy(string, zmq_msg_data(&message), size);
    string[size] = 0;

    zmq_msg_close(&message);
    return string;
}

int ssend(void* socket, const char* string, bool more) {
    zmq_msg_t message;
    int size = strlen(string);
    zmq_msg_init_size(&message, size);
    memcpy(zmq_msg_data(&message), string, size);

    int rc = zmq_send(socket, &message, more ? ZMQ_SNDMORE : 0);

    if (rc == -1) {
        fprintf(stderr, "error: failed to send to ZMQ socket (errno=%d)\n", errno);
        exit(1);
    }

    zmq_msg_close(&message);
    return rc;
}
