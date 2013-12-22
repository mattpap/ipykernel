#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "communication.h"
#include "handlers.h"
#include "msg.h"

void* eventloop(void* socket) {
    Msg msg;

    while (1) {
        msg_recv(socket, &msg);

        switch (msg.header.msg_type) {
            case msg_execute_request:
                execute_handler(socket, &msg);
                break;
            case msg_complete_request:
                complete_handler(socket, &msg);
                break;
            case msg_kernel_info_request:
                kernel_info_handler(socket, &msg);
                break;
            case msg_connect_request:
                connect_handler(socket, &msg);
                break;
            case msg_shutdown_request:
                shutdown_handler(socket, &msg);
                break;
            case msg_object_info_request:
                object_info_handler(socket, &msg);
                break;
            case msg_history_request:
                history_handler(socket, &msg);
                break;
            default:
                fprintf(stderr, "error: unsupported message type: %s", dump_msg_type(msg.header.msg_type));
                exit(1);
        }
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
