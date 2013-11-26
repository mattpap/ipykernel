#include <openssl/hmac.h>
#include <jansson.h>
#include <zmq.h>

int main(int argc, char** argv) {
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(socket, "tcp://*:5555");

    while (1) {
        zmq_msg_t message;
        zmq_msg_init(&message);
        zmq_msg_recv(&message, socket, 0);
        //  TODO
        zmq_msg_close(&message);
        if (!zmq_msg_more(&message))
            break;
    }

    return 0;
}
