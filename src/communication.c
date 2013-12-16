#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "util.h"
#include "msg.h"

static const char* DELIMITER = "<IDS|MSG>";

void msg_recv(void* socket, Msg* msg) {
    char* ident;

    memset(msg, 0, sizeof(Msg));

    while (1) {
        ident = srecv(socket);

        if (strcmp(ident, DELIMITER) == 0)
            break;
        else {
            msg->idents = realloc(msg->idents, (++msg->num_idents)*sizeof(char*));
            msg->idents[msg->num_idents-1] = ident;
        }
    }

    char* signature = srecv(socket);
    char* header = srecv(socket);
    char* parent_header = srecv(socket);
    char* metadata = srecv(socket);
    char* content = srecv(socket);

    if (options.verbose) {
        fprintf(stdout, "MSG received:\n");
        fprintf(stdout, "signature: %s\n", signature);
        fprintf(stdout, "header: %s\n", header);
        fprintf(stdout, "parent_header: %s\n", parent_header);
        fprintf(stdout, "metadata: %s\n", metadata);
        fprintf(stdout, "content: %s\n", content);
    }

    char* computed = hmac(header, parent_header, metadata, content, NULL);

    if (strcmp(signature, computed) != 0) {
        fprintf(stderr, "error: invalid HMAC signature, %s != %s\n", signature, computed);
        exit(1);
    }

    load_header(loads(header), &msg->header);
    //load_parent_header(parent_header, &msg->parent_header);
    load_metadata(loads(metadata), &msg->metadata);
    load_content(loads(content), msg->header.msg_type, &msg->content);

    free(computed);
    free(content);
    free(metadata);
    free(parent_header);
    free(header);
    free(signature);
}

void msg_send(void* socket, Msg* msg) {
    int i;
    char* header;
    char* parent_header;
    char* metadata;
    char* content;
    char* signature;

    header = dumps(dump_header(&msg->header));
    parent_header = dumps(dump_header(msg->parent_header));
    metadata = dumps(dump_metadata(&msg->metadata));
    content = dumps(dump_content(msg->header.msg_type, &msg->content));

    signature = hmac(header, parent_header, metadata, content, NULL);

    for (i = 0; i < msg->num_idents; i++)
        ssend(socket, msg->idents[i], true);

    ssend(socket, DELIMITER, true);
    ssend(socket, signature, true);
    ssend(socket, header, true);
    ssend(socket, parent_header, true);
    ssend(socket, metadata, true);
    ssend(socket, content, false);

    free(signature);
    free(content);
    free(metadata);
    free(parent_header);
    free(header);
}

static char* status_idents[] = { "status" };
static char* status_username = "aldor_kernel";

void send_status(ExecutionState state) {
    Msg msg;

    msg.idents = status_idents;
    msg.num_idents = 1;
    msg.header.msg_id = uuid4();
    msg.header.username = status_username;
    msg.header.session = uuid4();
    msg.header.msg_type = msg_status;
    msg.parent_header = NULL;
    msg.metadata = NULL;
    msg.content.status.execution_state = state;

    msg_send(sockets.publish, &msg);

    free(msg.header.msg_id);
    free(msg.header.session);
}

void send_ok(Msg* msg, int execution_count) {
    // TODO
}

void send_error(Msg* msg, int execution_count, char* error) {
    // TODO
}

void send_abort(Msg* msg, int execution_count) {
    // TODO
}

void send_stream(Msg* msg, char* name, char* data) {
    // TODO
}
