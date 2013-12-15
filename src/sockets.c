#include <stdlib.h>
#include <string.h>
#include <zmq.h>

#include "globals.h"
#include "sockets.h"
#include "concat.h"

char* endpoint(int port) {
    char str_port[32];
    sprintf(str_port, "%d", port);
    return concat(profile.transport, "://", profile.ip, ":", str_port, NULL);
}

void init_sockets(Sockets* sockets) {
    sockets->ctx = zmq_ctx_new();

    sockets->publish = zmq_socket(sockets->ctx, ZMQ_PUB);
    sockets->requests = zmq_socket(sockets->ctx, ZMQ_ROUTER);
    sockets->control = zmq_socket(sockets->ctx, ZMQ_ROUTER);
    sockets->stdin = zmq_socket(sockets->ctx, ZMQ_ROUTER);
    sockets->heartbeat = zmq_socket(sockets->ctx, ZMQ_REP);

    char* iopub = endpoint(profile.iopub_port);
    char* requests = endpoint(profile.shell_port);
    char* control = endpoint(profile.control_port);
    char* stdin = endpoint(profile.stdin_port);
    char* heartbeat = endpoint(profile.hb_port);

    zmq_bind(sockets->publish, iopub);
    zmq_bind(sockets->requests, requests);
    zmq_bind(sockets->control, control);
    zmq_bind(sockets->stdin, stdin);
    zmq_bind(sockets->heartbeat, heartbeat);

    free(heartbeat);
    free(stdin);
    free(control);
    free(requests);
    free(iopub);
}

void free_sockets(Sockets* sockets) {
    zmq_close(sockets->heartbeat);
    zmq_close(sockets->stdin);
    zmq_close(sockets->control);
    zmq_close(sockets->requests);
    zmq_close(sockets->publish);

    zmq_term(sockets->ctx);
}
