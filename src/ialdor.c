#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <openssl/hmac.h>
#include <jansson.h>
#include <zmq.h>

#include "profile.h"
#include "sockets.h"

int main(int argc, char** argv) {
    Profile profile;
    Sockets sockets;

    init_profile(&profile, NULL);
    init_sockets(&sockets, &profile);

    while (1) {
        zmq_msg_t message;
        zmq_msg_init(&message);
        zmq_msg_recv(&message, sockets.requests, 0);
        //  TODO
        zmq_msg_close(&message);
        if (!zmq_msg_more(&message))
            break;
    }

    return 0;
}
