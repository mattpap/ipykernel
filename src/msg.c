#include <jansson.h>

#include "msg.h"
#include "util.h"

#define pass

const char* msg_strof(MsgType msg_type) {
    return "";
}

MsgType msg_type_from_string(const char* msg_type) {
    if (strcmp(msg_type, "execute_request"))
        return msg_execute_request;
    else if (strcmp(msg_type, "execute_reply"))
        return msg_execute_reply;
    else if (strcmp(msg_type, "object_info_request"))
        return msg_object_info_request;
    else if (strcmp(msg_type, "object_info_reply"))
        return msg_object_info_reply;
    else if (strcmp(msg_type, "complete_request"))
        return msg_complete_request;
    else if (strcmp(msg_type, "complete_reply"))
        return msg_complete_reply;
    else if (strcmp(msg_type, "history_request"))
        return msg_history_request;
    else if (strcmp(msg_type, "history_reply"))
        return msg_history_reply;
    else if (strcmp(msg_type, "connect_request"))
        return msg_connect_request;
    else if (strcmp(msg_type, "connect_reply"))
        return msg_connect_reply;
    else if (strcmp(msg_type, "kernel_info_request"))
        return msg_kernel_info_request;
    else if (strcmp(msg_type, "kernel_info_reply"))
        return msg_kernel_info_reply;
    else if (strcmp(msg_type, "shutdown_request"))
        return msg_shutdown_request;
    else if (strcmp(msg_type, "shutdown_reply"))
        return msg_shutdown_reply;
    else if (strcmp(msg_type, "stream"))
        return msg_stream;
    else if (strcmp(msg_type, "display_data"))
        return msg_display_data;
    else if (strcmp(msg_type, "pyin"))
        return msg_pyin;
    else if (strcmp(msg_type, "pyout"))
        return msg_pyout;
    else if (strcmp(msg_type, "pyerr"))
        return msg_pyerr;
    else if (strcmp(msg_type, "status"))
        return msg_status;
    else if (strcmp(msg_type, "input_request"))
        return msg_input_request;
    else if (strcmp(msg_type, "input_reply"))
        return msg_input_reply;
    else {
        fprintf(stderr, "error: \"%s\" unsupported message type", msg_type);
        exit(1);
    }
}

void load_header(const json_t* json, Header* header) {
    // uuid_t msg_id;
    // char* username;
    // uuid_t session;
    // MsgType msg_type;
}

void load_execute_request(const json_t* json, ExecuteRequest* execute_request) {
    execute_request->code = json_get_string_key(json, "code");
    execute_request->silent = json_get_bool_key(json, "silent");
    execute_request->store_history = json_get_bool_key(json, "store_history");
    execute_request->user_variables = NULL;
    execute_request->user_expressions = NULL;
    execute_request->allow_stdin = json_get_bool_key(json, "allow_stdin");
}

void load_object_info_request(const json_t* json, ObjectInfoRequest* object_info_request) {
    object_info_request->oname = json_get_string_key(json, "oname");
    object_info_request->detail_level = json_get_integer_key(json, "detail_level");
}

void load_complete_request(const json_t* json, CompleteRequest* complete_request) {
    complete_request->text = json_get_string_key(json, "code");
    complete_request->line = json_get_string_key(json, "code");
    complete_request->block = json_get_string_key(json, "code");
    complete_request->cursor_pos = json_get_integer_key(json, "cursor_pos");
}

void load_history_request(const json_t* json, HistoryRequest* history_request) {
    history_request->output = json_get_bool_key(json, "output");
    history_request->raw = json_get_bool_key(json, "raw");
    char* hist_access_type = json_get_string_key(json, "hist_access_type");
    if (strcmp(hist_access_type, "range"))
        history_request->hist_access_type = hist_access_range;
    else if (strcmp(hist_access_type, "tail"))
        history_request->hist_access_type = hist_access_tail;
    else if (strcmp(hist_access_type, "search"))
        history_request->hist_access_type = hist_access_search;
    else {
        fprintf(stderr, "error: unexpected \"%s\" value for HistAccessType enumeration", hist_access_type);
        exit(1);
    }
    history_request->session = json_get_integer_key(json, "session");
    history_request->start = json_get_integer_key(json, "start");
    history_request->stop = json_get_integer_key(json, "stop");
    history_request->n = json_get_integer_key(json, "n");
    history_request->pattern = json_get_string_key(json, "pattern");
    history_request->unique = json_get_bool_key(json, "unique");
}

void load_connect_request(const json_t* json, ConnectRequest* connect_request) {
    pass;
}

void load_kernel_info_request(const json_t* json, KernelInfoRequest* kernel_info_request) {
    pass;
}

void load_shutdown_request(const json_t* json, ShutdownRequest* shutdown_request) {
    pass;
}

void load_input_reply(const json_t* json, InputReply* input_reply) {
    input_reply->value = json_get_string_key(json, "value");
}

void load_content(MsgType msg_type, const json_t* json, Content* content) {
    switch (msg_type) {
        case msg_execute_request:
            load_execute_request(json, &(content->execute_request));
        case msg_complete_request:
            load_complete_request(json, &(content->complete_request));
        case msg_kernel_info_request:
            load_kernel_info_request(json, &(content->kernel_info_request));
        case msg_object_info_request:
            load_object_info_request(json, &(content->object_info_request));
        case msg_connect_request:
            load_connect_request(json, &(content->connect_request));
        case msg_shutdown_request:
            load_shutdown_request(json, &(content->shutdown_request));
        case msg_history_request:
            load_history_request(json, &(content->history_request));
        case msg_input_reply:
            load_input_reply(json, &(content->input_reply));
        default:
            fprintf(stderr, "error: unexpected message type: \"%s\"", msg_strof(msg_type));
            exit(1);
    }
}

json_t* dump_execute_reply(ExecuteReply* execute_reply) {
    json_t* json = json_object();
    json_object_set(json, "execution_count", json_integer(execute_reply->execution_count));
    switch (execute_reply->status) {
        case status_ok:
            json_object_set(json, "status", json_string("ok"));
            json_object_set(json, "payload", json_array());
            json_object_set(json, "user_variables", json_null());
            json_object_set(json, "user_expressions", json_null());
        case status_error:
            json_object_set(json, "status", json_string("error"));
            json_object_set(json, "ename", json_string(execute_reply->execute_reply_data.error_reply.ename));
            json_object_set(json, "evalue", json_string(execute_reply->execute_reply_data.error_reply.evalue));
            json_object_set(json, "traceback", json_string_array(execute_reply->execute_reply_data.error_reply.traceback,
                                                                 execute_reply->execute_reply_data.error_reply.num_traceback));
        case status_abort:
            json_object_set(json, "status", json_string("abort"));
    }
    return json;
}

json_t* dump_object_info_reply(ObjectInfoReply* object_info_reply) {
    json_t* json = json_object();
    return json;
}

json_t* dump_complete_reply(CompleteReply* complete_reply) {
    json_t* json = json_object();
    return json;
}

json_t* dump_history_reply(HistoryReply* history_reply) {
    json_t* json = json_object();
    return json;
}

json_t* dump_connect_reply(ConnectReply* connect_reply) {
    json_t* json = json_object();
    return json;
}

json_t* dump_kernel_info_reply(KernelInfoReply* kernel_info_reply) {
    json_t* json = json_object();
    return json;
}

json_t* dump_shutdown_reply(ShutdownReply* shutdown_reply) {
    json_t* json = json_object();
    return json;
}

json_t* dump_stream(Stream* stream) {
    json_t* json = json_object();
    return json;
}

json_t* dump_display_data(DisplayData* display_data) {
    json_t* json = json_object();
    return json;
}

json_t* dump_pyin(PyIn* pyin) {
    json_t* json = json_object();
    return json;
}

json_t* dump_pyout(PyOut* pyout) {
    json_t* json = json_object();
    return json;
}

json_t* dump_pyerr(PyErr* pyerr) {
    json_t* json = json_object();
    return json;
}

json_t* dump_status(Status* status) {
    json_t* json = json_object();
    return json;
}

json_t* dump_input_request(InputRequest* input_request) {
    json_t* json = json_object();
    return json;
}
