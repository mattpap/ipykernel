#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "msg.h"
#include "communication.h"

void execute_handler(void* socket, Msg* msg) {
    int n = 0;
    send_status(state_busy);
    printf("%s\n", msg->content.execute_request.code);
    send_ok(msg, n);
    send_status(state_starting);
}

void complete_handler(void* socket, Msg* msg) {
    CompleteReply complete_reply = {
        .status = status_ok,
        .matches = { .list = NULL, .size = 0 },
        .matched_text = msg->content.complete_request.text,
    };
    Content content = { .complete_reply = complete_reply };
    send_reply(socket, msg, msg_complete_reply, &content);
}

static const char* aldor = "aldor";

void kernel_info_handler(void* socket, Msg* msg) {
    KernelInfoReply kernel_info_reply = {
        .protocol_version = { .major = 4, .minor = 0 },
        .ipython_version = NULL,
        .language_version = { .major = 1, .minor = 11 },
        .language = (char*)aldor,
    };
    Content content = { .kernel_info_reply = kernel_info_reply };
    send_reply(socket, msg, msg_kernel_info_reply, &content);
}

void connect_handler(void* socket, Msg* msg) {
    ConnectReply connect_reply = {
        .shell_port = profile.shell_port,
        .iopub_port = profile.iopub_port,
        .stdin_port = profile.stdin_port,
        .hb_port    = profile.hb_port,
    };
    Content content = { .connect_reply = connect_reply };
    send_reply(socket, msg, msg_connect_reply, &content);
}

void shutdown_handler(void* socket, Msg* msg) {
    ShutdownReply shutdown_reply = {
        .restart = msg->content.shutdown_request.restart,
    };
    Content content = { .shutdown_reply = shutdown_reply };
    send_reply(socket, msg, msg_shutdown_reply, &content);
    exit(0);
}

void object_info_handler(void* socket, Msg* msg) {
    ObjectInfoReply object_info_reply = {
        .name = msg->content.object_info_request.oname,
        .found = false,
    };
    Content content = { .object_info_reply = object_info_reply };
    send_reply(socket, msg, msg_object_info_reply, &content);
}

void history_handler(void* socket, Msg* msg) {
    HistoryReply history_reply = {
        .history = { .list = NULL, .size = 0 },
    };
    Content content = { .history_reply = history_reply };
    send_reply(socket, msg, msg_history_reply, &content);
}
