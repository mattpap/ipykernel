#include <stdlib.h>
#include <string.h>

#include "msg.h"
#include "json.h"

#define pass

static const char* msg_types[NUM_MSG_TYPE] = {
    "execute_request",
    "execute_reply",
    "object_info_request",
    "object_info_reply",
    "complete_request",
    "complete_reply",
    "history_request",
    "history_reply",
    "connect_request",
    "connect_reply",
    "kernel_info_request",
    "kernel_info_reply",
    "shutdown_request",
    "shutdown_reply",
    "stream",
    "display_data",
    "pyin",
    "pyout",
    "pyerr",
    "status",
    "input_request",
    "input_reply",
};

MsgType load_msg_type(const char* msg_type) {
    int i;

    for (i = 0; i < NUM_MSG_TYPE; i++) {
        if (strcmp(msg_type, msg_types[i]) == 0)
            return (MsgType)i;
    }

    fprintf(stderr, "error: \"%s\" unsupported message type\n", msg_type);
    exit(1);
}

const char* dump_msg_type(MsgType msg_type) {
    return msg_types[(int)msg_type];
}

static const char* execution_statuses[NUM_EXECUTION_STATUS] = {
    "ok",
    "error",
    "abort",
};

ExecutionStatus load_execution_status(const char* execution_status) {
    int i;

    for (i = 0; i < NUM_EXECUTION_STATUS; i++) {
        if (strcmp(execution_status, execution_statuses[i]) == 0)
            return (ExecutionStatus)i;
    }

    fprintf(stderr, "error: \"%s\" unsupported execution status\n", execution_status);
    exit(1);
}

const char* dump_execution_status(ExecutionStatus execution_status) {
    return execution_statuses[(int)execution_status];
}

static const char* hist_access_types[NUM_HIST_ACCESS_TYPE] = {
    "range",
    "tail",
    "search",
};

HistAccessType load_hist_access_type(const char* hist_access_type) {
    int i;

    for (i = 0; i < NUM_HIST_ACCESS_TYPE; i++) {
        if (strcmp(hist_access_type, hist_access_types[i]) == 0)
            return (HistAccessType)i;
    }

    fprintf(stderr, "error: \"%s\" unsupported history access type\n", hist_access_type);
    exit(1);
}

const char* dump_hist_access_type(HistAccessType hist_access_type) {
    return hist_access_types[(int)hist_access_type];
}

static const char* execution_states[NUM_EXECUTION_STATE] = {
    "busy",
    "idle",
    "starting",
};

ExecutionState load_execution_state(const char* execution_state) {
    int i;

    for (i = 0; i < NUM_EXECUTION_STATE; i++) {
        if (strcmp(execution_state, execution_states[i]) == 0)
            return (ExecutionState)i;
    }

    fprintf(stderr, "error: \"%s\" unsupported execution state\n", execution_state);
    exit(1);
}

const char* dump_execution_state(ExecutionState execution_state) {
    return execution_states[(int)execution_state];
}

static void load_string_list(const json_t* json, StringList* strings) {
    if (!json_is_array(json)) {
        fprintf(stderr, "error: expected a JSON array, got %s\n", json_strof(json));
        exit(1);
    }

    strings->list = NULL;
    strings->size = json_array_size(json);

    if (strings->size != 0) {
        strings->list = malloc(strings->size*sizeof(char*));

        size_t i;
        json_t* value;

        json_array_foreach((json_t*)json, i, value) {
            if (!json_is_string(value)) {
                fprintf(stderr, "error: expected value of type string at %ld index, got %s\n", i, json_strof(value));
                exit(1);
            }

            strings->list[i] = strdup(json_string_value(value));
        }
    }
}

static json_t* dump_string_list(const StringList* strings) {
    size_t i;
    json_t* array = json_array();
    for (i = 0; i < strings->size; i++)
        json_array_append_new(array, json_string(strings->list[i]));
    return array;
}

static void load_dict(const json_t* json, Dict* dict) {
    if (!json_is_object(json)) {
        fprintf(stderr, "error: expected a JSON object, got %s\n", json_strof(json));
        exit(1);
    }

    dict->list = NULL;
    dict->size = json_object_size(json);

    if (dict->size != 0) {
        dict->list = malloc(dict->size*sizeof(char*));

        size_t i = 0;
        const char* key;
        json_t* value;
        KeyValue* kv;

        json_object_foreach((json_t*)json, key, value) {
            if (!json_is_string(value)) {
                fprintf(stderr, "error: expected value of type string for \"%s\" key, got %s\n", key, json_strof(value));
                exit(1);
            }

            kv = &dict->list[i++];

            kv->key = strdup(key);
            kv->value = strdup(json_string_value(value));
        }
    }
}

static json_t* dump_dict(const Dict* dict) {
    size_t i;
    json_t* obj = json_object();
    for (i = 0; i < dict->size; i++)
        json_object_set(obj, dict->list[i].key, json_string(dict->list[i].value));
    return obj;
}

static void load_execute_request(const json_t* json, ExecuteRequest* execute_request) {
    execute_request->code = json_get_string_key(json, "code");
    execute_request->silent = json_get_bool_key(json, "silent");
    execute_request->store_history = json_get_bool_key(json, "store_history");
    load_string_list(json_object_get(json, "user_variables"), &execute_request->user_variables);
    load_dict(json_object_get(json, "user_expressions"), &execute_request->user_expressions);
    execute_request->allow_stdin = json_get_bool_key(json, "allow_stdin");
}

static void load_object_info_request(const json_t* json, ObjectInfoRequest* object_info_request) {
    object_info_request->oname = json_get_string_key(json, "oname");
    object_info_request->detail_level = json_get_integer_key(json, "detail_level");
}

static void load_complete_request(const json_t* json, CompleteRequest* complete_request) {
    complete_request->text = json_get_string_key(json, "text");
    complete_request->line = json_get_string_key(json, "line");
    complete_request->block = json_get_string_key(json, "block");
    complete_request->cursor_pos = json_get_integer_key(json, "cursor_pos");
}

static void load_history_request(const json_t* json, HistoryRequest* history_request) {
    history_request->output = json_get_bool_key(json, "output");
    history_request->raw = json_get_bool_key(json, "raw");
    history_request->hist_access_type = load_hist_access_type(json_get_string_key(json, "hist_access_type"));
    history_request->session = json_get_integer_key(json, "session");
    history_request->start = json_get_integer_key(json, "start");
    history_request->stop = json_get_integer_key(json, "stop");
    history_request->n = json_get_integer_key(json, "n");
    history_request->pattern = json_get_string_key(json, "pattern");
    history_request->unique = json_get_bool_key(json, "unique");
}

static void load_connect_request(const json_t* json, ConnectRequest* connect_request) {
    pass;
}

static void load_kernel_info_request(const json_t* json, KernelInfoRequest* kernel_info_request) {
    pass;
}

static void load_shutdown_request(const json_t* json, ShutdownRequest* shutdown_request) {
    shutdown_request->restart = json_get_bool_key(json, "restart");
}

static void load_input_reply(const json_t* json, InputReply* input_reply) {
    input_reply->value = json_get_string_key(json, "value");
}

static json_t* dump_execute_reply(const ExecuteReply* execute_reply) {
    json_t* json = json_object();
    json_object_set(json, "execution_count", json_integer(execute_reply->execution_count));
    json_object_set(json, "status", json_string(dump_execution_status(execute_reply->status)));
    switch (execute_reply->status) {
        case status_ok:
            json_object_set(json, "payload", dump_dict(&execute_reply->ok_reply.payload));
            json_object_set(json, "user_variables", dump_string_list(&execute_reply->ok_reply.user_variables));
            json_object_set(json, "user_expressions", dump_dict(&execute_reply->ok_reply.user_expressions));
            break;
        case status_error:
            json_object_set(json, "ename", json_string(execute_reply->error_reply.ename));
            json_object_set(json, "evalue", json_string(execute_reply->error_reply.evalue));
            json_object_set(json, "traceback", dump_string_list(&execute_reply->error_reply.traceback));
            break;
        case status_abort:
            break;
    }
    return json;
}

static json_t* dump_complete_reply(const CompleteReply* complete_reply) {
    json_t* json = json_object();
    json_object_set(json, "matches", dump_string_list(&complete_reply->matches));
    json_object_set(json, "matched_text", json_string(complete_reply->matched_text));
    json_object_set(json, "status", json_string(dump_execution_status(complete_reply->status)));
    return json;
}

static json_t* dump_object_info_reply(const ObjectInfoReply* object_info_reply) {
    json_t* json = json_object();
    json_object_set(json, "name", json_string(object_info_reply->name));
    json_object_set(json, "found", json_boolean(object_info_reply->found));
    if (object_info_reply->found) {
        json_object_set(json, "ismagic", json_boolean(object_info_reply->found_reply.ismagic));
        json_object_set(json, "isalias", json_boolean(object_info_reply->found_reply.isalias));
        json_object_set(json, "namespace", json_string(object_info_reply->found_reply.namespace));
        json_object_set(json, "type_name", json_string(object_info_reply->found_reply.type_name));
        json_object_set(json, "string_form", json_string(object_info_reply->found_reply.string_form));
        json_object_set(json, "base_class", json_string(object_info_reply->found_reply.base_class));
        json_object_set(json, "length", json_string(object_info_reply->found_reply.length));
        json_object_set(json, "file", json_string(object_info_reply->found_reply.file));
        json_object_set(json, "definition", json_string(object_info_reply->found_reply.definition));
        json_t* argspec = json_object();
        json_object_set(argspec, "args", dump_string_list(&object_info_reply->found_reply.argspec.args));
        json_object_set(argspec, "varargs", json_string(object_info_reply->found_reply.argspec.varargs));
        json_object_set(argspec, "varkw", json_string(object_info_reply->found_reply.argspec.varkw));
        json_object_set(argspec, "defaults", dump_string_list(&object_info_reply->found_reply.argspec.defaults));
        json_object_set(json, "argspec", argspec);
        json_object_set(json, "init_definition", json_string(object_info_reply->found_reply.init_definition));
        json_object_set(json, "docstring", json_string(object_info_reply->found_reply.docstring));
        json_object_set(json, "init_docstring", json_string(object_info_reply->found_reply.init_docstring));
        json_object_set(json, "class_docstring", json_string(object_info_reply->found_reply.class_docstring));
        json_object_set(json, "call_def", json_string(object_info_reply->found_reply.call_def));
        json_object_set(json, "call_docstring", json_string(object_info_reply->found_reply.call_docstring));
        json_object_set(json, "source", json_string(object_info_reply->found_reply.source));
    }
    return json;
}

static json_t* dump_history_reply(const HistoryReply* history_reply) {
    size_t i;
    json_t* json = json_object();
    json_t* history = json_array();
    json_t* item;
    HistoryItem history_item;

    for (i = 0; i < history_reply->history.size; i++) {
        history_item = history_reply->history.list[i];
        item = json_object();
        json_object_set(item, "session", json_integer(history_item.session));
        json_object_set(item, "line_number", json_integer(history_item.line_number));
        json_object_set(item, "input", json_string(history_item.input));
        json_object_set(item, "output", json_string(history_item.output));
        json_array_append_new(history, item);
    }

    json_object_set(json, "history", history);
    return json;
}

static json_t* dump_connect_reply(const ConnectReply* connect_reply) {
    json_t* json = json_object();
    json_object_set(json, "shell_port", json_integer(connect_reply->shell_port));
    json_object_set(json, "iopub_port", json_integer(connect_reply->iopub_port));
    json_object_set(json, "stdin_port", json_integer(connect_reply->stdin_port));
    json_object_set(json, "hb_port", json_integer(connect_reply->hb_port));
    return json;
}

static json_t* dump_kernel_info_reply(const KernelInfoReply* kernel_info_reply) {
    json_t* json = json_object();

    json_t* protocol_version = json_array();
    json_array_append_new(protocol_version, json_integer(kernel_info_reply->protocol_version.major));
    json_array_append_new(protocol_version, json_integer(kernel_info_reply->protocol_version.minor));

    json_t* ipython_version;
    if (kernel_info_reply->ipython_version != NULL) {
        ipython_version = json_array();
        json_array_append_new(ipython_version, json_integer(kernel_info_reply->ipython_version->major));
        json_array_append_new(ipython_version, json_integer(kernel_info_reply->ipython_version->minor));
        json_array_append_new(ipython_version, json_integer(kernel_info_reply->ipython_version->maintenance));
        json_array_append_new(ipython_version, json_string(kernel_info_reply->ipython_version->build));
    } else {
        ipython_version = json_null();
    }

    json_t* language_version = json_array();
    json_array_append_new(language_version, json_integer(kernel_info_reply->language_version.major));
    json_array_append_new(language_version, json_integer(kernel_info_reply->language_version.minor));

    json_object_set(json, "protocol_version", protocol_version);
    json_object_set(json, "ipython_version", ipython_version);
    json_object_set(json, "language_version", language_version);
    json_object_set(json, "language", json_string(kernel_info_reply->language));
    return json;
}

static json_t* dump_shutdown_reply(const ShutdownReply* shutdown_reply) {
    json_t* json = json_object();
    json_object_set(json, "restart", json_boolean(shutdown_reply->restart));
    return json;
}

static json_t* dump_stream(const Stream* stream) {
    json_t* json = json_object();
    json_object_set(json, "name", json_string(stream->name));
    json_object_set(json, "data", json_string(stream->data));
    return json;
}

static json_t* dump_display_data(const DisplayData* display_data) {
    json_t* json = json_object();
    json_object_set(json, "source", json_string(display_data->source));
    json_object_set(json, "data", dump_dict(&display_data->data));
    json_object_set(json, "metadata", dump_dict(&display_data->metadata));
    return json;
}

static json_t* dump_pyin(const PyIn* pyin) {
    json_t* json = json_object();
    json_object_set(json, "code", json_string(pyin->code));
    json_object_set(json, "execution_count", json_integer(pyin->execution_count));
    return json;
}

static json_t* dump_pyout(const PyOut* pyout) {
    json_t* json = json_object();
    json_object_set(json, "execution_count", json_integer(pyout->execution_count));
    json_object_set(json, "data", dump_dict(&pyout->data));
    json_object_set(json, "metadata", dump_dict(&pyout->metadata));
    return json;
}

static json_t* dump_pyerr(const PyErr* pyerr) {
    json_t* json = json_object();
    json_object_set(json, "execution_count", json_integer(pyerr->execution_count));
    json_object_set(json, "ename", json_string(pyerr->ename));
    json_object_set(json, "evalue", json_string(pyerr->evalue));
    json_object_set(json, "traceback", dump_string_list(&pyerr->traceback));
    return json;
}

static json_t* dump_status(const Status* status) {
    json_t* json = json_object();
    json_object_set(json, "execution_state", json_string(dump_execution_state(status->execution_state)));
    return json;
}

static json_t* dump_input_request(const InputRequest* input_request) {
    json_t* json = json_object();
    json_object_set(json, "prompt", json_string(input_request->prompt));
    return json;
}

void load_header(const json_t* json, Header* header) {
    header->msg_id = json_get_string_key(json, "msg_id");
    header->username = json_get_string_key(json, "username");
    header->session = json_get_string_key(json, "session");
    header->msg_type = load_msg_type(json_get_string_key(json, "msg_type"));
}

void load_optional_header(const json_t* json, Header** header) {
    if (json_object_size(json) == 0)
        *header = NULL;
    else
        load_header(json, *header);
}

json_t* dump_header(const Header* header) {
    json_t* json = json_object();

    if (header != NULL) {
        json_object_set(json, "msg_id", json_string(header->msg_id));
        json_object_set(json, "username", json_string(header->username));
        json_object_set(json, "session", json_string(header->session));
        json_object_set(json, "msg_type", json_string(dump_msg_type(header->msg_type)));
    }

    return json;
}

void load_metadata(const json_t* json, Dict* metadata) {
    load_dict(json, metadata);
}

json_t* dump_metadata(const Dict* metadata) {
    return dump_dict(metadata);
}

void load_content(const json_t* json, MsgType msg_type, Content* content) {
    switch (msg_type) {
        case msg_execute_request:
            load_execute_request(json, &content->execute_request);
            break;
        case msg_complete_request:
            load_complete_request(json, &content->complete_request);
            break;
        case msg_kernel_info_request:
            load_kernel_info_request(json, &content->kernel_info_request);
            break;
        case msg_object_info_request:
            load_object_info_request(json, &content->object_info_request);
            break;
        case msg_connect_request:
            load_connect_request(json, &content->connect_request);
            break;
        case msg_shutdown_request:
            load_shutdown_request(json, &content->shutdown_request);
            break;
        case msg_history_request:
            load_history_request(json, &content->history_request);
            break;
        case msg_input_reply:
            load_input_reply(json, &content->input_reply);
            break;
        default:
            fprintf(stderr, "error: unexpected message type: \"%s\"\n", dump_msg_type(msg_type));
            exit(1);
    }
}

json_t* dump_content(MsgType msg_type, const Content* content) {
    switch (msg_type) {
        case msg_execute_reply:     return dump_execute_reply(&content->execute_reply);
        case msg_complete_reply:    return dump_complete_reply(&content->complete_reply);
        case msg_kernel_info_reply: return dump_kernel_info_reply(&content->kernel_info_reply);
        case msg_object_info_reply: return dump_object_info_reply(&content->object_info_reply);
        case msg_connect_reply:     return dump_connect_reply(&content->connect_reply);
        case msg_shutdown_reply:    return dump_shutdown_reply(&content->shutdown_reply);
        case msg_history_reply:     return dump_history_reply(&content->history_reply);
        case msg_input_request:     return dump_input_request(&content->input_request);
        case msg_stream:            return dump_stream(&content->stream);
        case msg_display_data:      return dump_display_data(&content->display_data);
        case msg_pyin:              return dump_pyin(&content->pyin);
        case msg_pyout:             return dump_pyout(&content->pyout);
        case msg_pyerr:             return dump_pyerr(&content->pyerr);
        case msg_status:            return dump_status(&content->status);
        default:
            fprintf(stderr, "error: unexpected message type: \"%s\"\n", dump_msg_type(msg_type));
            exit(1);
    }
}
