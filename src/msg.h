#ifndef __IALDOR_MSG_H__
#define __IALDOR_MSG_H__

#include <stdbool.h>
#include <uuid/uuid.h>

typedef enum ExecutionStatus {
    status_ok = 0,
    status_error,
    status_abort,
} ExecutionStatus;

typedef enum HistAccessType {
    hist_access_range = 0,
    hist_access_tail,
    hist_access_search,
} HistAccessType;

typedef enum ExecutionState {
    state_busy = 0,
    state_idle,
    state_starting,
} ExecutionState;

#define NUM_EXECUTION_STATUS (status_abort+1)
#define NUM_HIST_ACCESS_TYPE (hist_access_search+1)
#define NUM_EXECUTION_STATE  (state_starting+1)

typedef struct ExecuteRequest {
    // Source code to be executed by the kernel, one or more lines.
    char* code;

    // A boolean flag which, if True, signals the kernel to execute
    // this code as quietly as possible.  This means that the kernel
    // will compile the code with 'exec' instead of 'single' (so
    // sys.displayhook will not fire), forces store_history to be False,
    // and will *not*:
    //   - broadcast exceptions on the PUB socket
    //   - do any logging
    //
    // The default is False.
    bool silent;

    // A boolean flag which, if True, signals the kernel to populate history
    // The default is True if silent is False.  If silent is True, store_history
    // is forced to be False.
    bool store_history;

    // A list of variable names from the user's namespace to be retrieved.
    // What returns is a rich representation of each variable (dict keyed by name).
    // See the display_data content for the structure of the representation data.
    char** user_variables;

    // Similarly, a dict mapping names to expressions to be evaluated in the
    // user's dict.
    void* user_expressions;

    // Some frontends (e.g. the Notebook) do not support stdin requests. If
    // raw_input is called from code executed from such a frontend, a
    // StdinNotImplementedError will be raised.
    bool allow_stdin;
} ExecuteRequest;

typedef struct ExecuteOkReply {
    // 'payload' will be a list of payload dicts.
    // Each execution payload is a dict with string keys that may have been
    // produced by the code being executed.  It is retrieved by the kernel at
    // the end of the execution and sent back to the front end, which can take
    // action on it as needed.  See main text for further details.
    void* payload;

    // Results for the user_variables and user_expressions.
    char** user_variables;
    void* user_expressions;
} ExecuteOkReply;

typedef struct ExecuteErrorReply {
    // Exception name, as a string
    char* ename;
    // Exception value, as a string
    char* evalue;

    // The traceback will contain a list of frames, represented each as a
    // string.  For now we'll stick to the existing design of ultraTB, which
    // controls exception level of detail statefully.  But eventually we'll
    // want to grow into a model where more information is collected and
    // packed into the traceback object, with clients deciding how little or
    // how much of it to unpack.  But for now, let's start with a simple list
    // of strings, since that requires only minimal changes to ultratb as
    // written.
    char** traceback;
    int num_traceback;
} ExecuteErrorReply;

typedef struct ExecuteAbortReply {
} ExecuteAbortReply;

typedef struct ExecuteReply {
    // One of: 'ok' OR 'error' OR 'abort'
    ExecutionStatus status;

    // The global kernel counter that increases by one with each request that
    // stores history.  This will typically be used by clients to display
    // prompt numbers to the user.  If the request did not store history, this will
    // be the current value of the counter in the kernel.
    int execution_count;

    union {
        ExecuteOkReply ok_reply;
        ExecuteErrorReply error_reply;
        ExecuteAbortReply abort_reply;
    } execute_reply_data;
} ExecuteReply;

typedef struct ObjectInfoRequest {
    // The (possibly dotted) name of the object to be searched in all
    // relevant namespaces
    char* oname;

    // The level of detail desired.  The default (0) is equivalent to typing
    // 'x?' at the prompt, 1 is equivalent to 'x??'.
    int detail_level;
} ObjectInfoRequest;

typedef struct ArgSpec {
    // The names of all the arguments
    char** args;
    // The name of the varargs (*args), if any
    char* varargs;
    // The name of the varkw (**kw), if any
    char* varkw;
    // The values (as strings) of all default arguments.  Note
    // that these must be matched *in reverse* with the 'args'
    // list above, since the first positional args have no default
    // value at all.
    char** defaults;
} ArgSpec;

typedef struct ObjectInfoNotFoundReply {
} ObjectInfoNotFoundReply;

typedef struct ObjectInfoFoundReply {
    // Flags for magics and system aliases
    bool ismagic;
    bool isalias;

    // The name of the namespace where the object was found ('builtin',
    // 'magics', 'alias', 'interactive', etc.)
    char* namespace;

    // The type name will be type.__name__ for normal Python objects, but it
    // can also be a string like 'Magic function' or 'System alias'
    char* type_name;

    // The string form of the object, possibly truncated for length if
    // detail_level is 0
    char* string_form;

    // For objects with a __class__ attribute this will be set
    char* base_class;

    // For objects with a __len__ attribute this will be set
    char* length;

    // If the object is a function, class or method whose file we can find,
    // we give its full path
    char* file;

    // For pure Python callable objects, we can reconstruct the object
    // definition line which provides its call signature.  For convenience this
    // is returned as a single 'definition' field, but below the raw parts that
    // compose it are also returned as the argspec field.
    char* definition;

    // The individual parts that together form the definition string.  Clients
    // with rich display capabilities may use this to provide a richer and more
    // precise representation of the definition line (e.g. by highlighting
    // arguments based on the user's cursor position).  For non-callable
    // objects, this field is empty.
    ArgSpec argspec;

    // For instances, provide the constructor signature (the definition of
    // the __init__ method):
    char* init_definition;

    // Docstrings: for any object (function, method, module, package) with a
    // docstring, we show it.  But in addition, we may provide additional
    // docstrings.  For example, for instances we will show the constructor
    // and class docstrings as well, if available.
    char* docstring;

    // For instances, provide the constructor and class docstrings
    char* init_docstring;
    char* class_docstring;

    // If it's a callable object whose call method has a separate docstring and
    // definition line:
    char* call_def;
    char* call_docstring;

    // If detail_level was 1, we also try to find the source code that
    // defines the object, if possible.  The string 'None' will indicate
    // that no source was found.
    char* source;
} ObjectInfoFoundReply;

typedef struct ObjectInfoReply {
    // The name the object was requested under
    char* name;

    // Boolean flag indicating whether the named object was found or not.  If
    // it's false, all other fields will be empty.
    bool found;

    union {
        ObjectInfoNotFoundReply notfound_reply;
        ObjectInfoFoundReply found_reply;
    } object_info_reply_data;
} ObjectInfoReply;

typedef struct CompleteRequest {
    // The text to be completed, such as 'a.is'
    // this may be an empty string if the frontend does not do any lexing,
    // in which case the kernel must figure out the completion
    // based on 'line' and 'cursor_pos'.
    char* text;

    // The full line, such as 'print a.is'.  This allows completers to
    // make decisions that may require information about more than just the
    // current word.
    char* line;

    // The entire block of text where the line is.  This may be useful in the
    // case of multiline completions where more context may be needed.  Note: if
    // in practice this field proves unnecessary, remove it to lighten the
    // messages.
    char* block;

    // The position of the cursor where the user hit 'TAB' on the line.
    int cursor_pos;
} CompleteRequest;

typedef struct CompleteReply {
    // The list of all matches to the completion request, such as
    // ['a.isalnum', 'a.isalpha'] for the above example.
    struct {
        char** list;
        size_t size;
    } matches;

    // the substring of the matched text
    // this is typically the common prefix of the matches,
    // and the text that is already in the block that would be replaced by the full completion.
    // This would be 'a.is' in the above example.
    char* matched_text;

    // status should be 'ok' unless an exception was raised during the request,
    // in which case it should be 'error', along with the usual error message content
    // in other messages.
    ExecutionStatus status;
} CompleteReply;

typedef struct HistoryRequest {
    // If True, also return output history in the resulting dict.
    bool output;

    // If True, return the raw input history, else the transformed input.
    bool raw;

    // So far, this can be 'range', 'tail' or 'search'.
    HistAccessType hist_access_type;

    // If hist_access_type is 'range', get a range of input cells. session can
    // be a positive session number, or a negative number to count back from
    // the current session.
    int session;

    // start and stop are line numbers within that session.
    int start;
    int stop;

    // If hist_access_type is 'tail' or 'search', get the last n cells.
    int n;

    // If hist_access_type is 'search', get cells matching the specified glob
    // pattern (with * and ? as wildcards).
    char* pattern;

    // If hist_access_type is 'search' and unique is true, do not
    // include duplicated history.  Default is false.
    bool unique;
} HistoryRequest;

typedef struct HistoryItem {
    int session;
    int line_number;
    char* input;
    char* output;
} HistoryItem;

typedef struct HistoryReply {
    // A list of 3 tuples, either:
    // (session, line_number, input) or
    // (session, line_number, (input, output)),
    // depending on whether output was False or True, respectively.
    struct {
        HistoryItem* list;
        size_t size;
    } history;
} HistoryReply;

typedef struct ConnectRequest {
} ConnectRequest;

typedef struct ConnectReply {
    // The port the shell ROUTER socket is listening on.
    int shell_port;
    // The port the PUB socket is listening on.
    int iopub_port;
    // The port the stdin ROUTER socket is listening on.
    int stdin_port;
    // The port the heartbeat socket is listening on.
    int hb_port;
} ConnectReply;

typedef struct KernelInfoRequest {
} KernelInfoRequest;

typedef struct ProtocolVersion {
    int major;
    int minor;
} ProtocolVersion;

typedef struct IPythonVersion {
    int major;
    int minor;
    int maintenance;
    char* build;
} IPythonVersion;

typedef struct LanguageVersion {
    int major;
    int minor;
} LanguageVersion;

typedef struct KernelInfoReply {
    // Version of messaging protocol (mandatory).
    // The first integer indicates major version.  It is incremented when
    // there is any backward incompatible change.
    // The second integer indicates minor version.  It is incremented when
    // there is any backward compatible change.
    ProtocolVersion protocol_version;

    // IPython version number (optional).
    // Non-python kernel backend may not have this version number.
    // The last component is an extra field, which may be 'dev' or
    // 'rc1' in development version.  It is an empty string for
    // released version.
    IPythonVersion* ipython_version;

    // Language version number (mandatory).
    // It is Python version number (e.g., [2, 7, 3]) for the kernel
    // included in IPython.
    LanguageVersion language_version;

    // Programming language in which kernel is implemented (mandatory).
    // Kernel included in IPython returns 'python'.
    char* language;
} KernelInfoReply;

typedef struct ShutdownRequest {
    // whether the shutdown is final, or precedes a restart
    bool restart;
} ShutdownRequest;

typedef struct ShutdownReply {
    // whether the shutdown is final, or precedes a restart
    bool restart;
} ShutdownReply;

typedef struct Stream {
    // The name of the stream is one of 'stdout', 'stderr'
    char* name;

    // The data is an arbitrary string to be written to that stream
    char* data;
} Stream;

typedef struct DisplayData {
    // Who create the data
    char* source;

    // The data dict contains key/value pairs, where the kids are MIME
    // types and the values are the raw data of the representation in that
    // format.
    void* data;

    // Any metadata that describes the data
    void* metadata;
} DisplayData;

typedef struct PyIn {
    // Source code to be executed, one or more lines
    char* code;

    // The counter for this execution is also provided so that clients can
    // display it, since IPython automatically creates variables called _iN
    // (for input prompt In[N]).
    int execution_count;
} PyIn;

typedef struct PyOut {
    // The counter for this execution is also provided so that clients can
    // display it, since IPython automatically creates variables called _N
    // (for prompt N).
    int execution_count;

    // data and metadata are identical to a display_data message.
    // the object being displayed is that passed to the display hook,
    // i.e. the *result* of the execution.
    void* data;
    void* metadata;
} PyOut;

typedef struct PyErr {
    int execution_count;

    // Exception name, as a string
    char* ename;
    // Exception value, as a string
    char* evalue;

    // The traceback will contain a list of frames, represented each as a
    // string.  For now we'll stick to the existing design of ultraTB, which
    // controls exception level of detail statefully.  But eventually we'll
    // want to grow into a model where more information is collected and
    // packed into the traceback object, with clients deciding how little or
    // how much of it to unpack.  But for now, let's start with a simple list
    // of strings, since that requires only minimal changes to ultratb as
    // written.
    char** traceback;
} PyErr;

typedef struct Status {
    // When the kernel starts to execute code, it will enter the 'busy'
    // state and when it finishes, it will enter the 'idle' state.
    // The kernel will publish state 'starting' exactly once at process startup.
    ExecutionState execution_state;
} Status;

typedef struct InputRequest {
    char* prompt;
} InputRequest;

typedef struct InputReply {
    char* value;
} InputReply;

typedef enum MsgType {
    msg_execute_request = 0,
    msg_execute_reply,
    msg_object_info_request,
    msg_object_info_reply,
    msg_complete_request,
    msg_complete_reply,
    msg_history_request,
    msg_history_reply,
    msg_connect_request,
    msg_connect_reply,
    msg_kernel_info_request,
    msg_kernel_info_reply,
    msg_shutdown_request,
    msg_shutdown_reply,
    msg_stream,
    msg_display_data,
    msg_pyin,
    msg_pyout,
    msg_pyerr,
    msg_status,
    msg_input_request,
    msg_input_reply,
} MsgType;

#define NUM_MSG_TYPE (msg_input_reply+1)

typedef struct Header {
    char* msg_id;
    char* username;
    char* session;
    MsgType msg_type;
} Header;

typedef union Content {
    ExecuteRequest execute_request;
    ExecuteReply execute_reply;
    ObjectInfoRequest object_info_request;
    ObjectInfoReply object_info_reply;
    CompleteRequest complete_request;
    CompleteReply complete_reply;
    HistoryRequest history_request;
    HistoryReply history_reply;
    ConnectRequest connect_request;
    ConnectReply connect_reply;
    KernelInfoRequest kernel_info_request;
    KernelInfoReply kernel_info_reply;
    ShutdownRequest shutdown_request;
    ShutdownReply shutdown_reply;
    Stream stream;
    DisplayData display_data;
    PyIn pyin;
    PyOut pyout;
    PyErr pyerr;
    Status status;
    InputRequest input_request;
    InputReply input_reply;
} Content;

typedef struct Msg {
    struct {
        char** list;
        int size;
    } idents;
    Header header;
    Header* parent_header;
    void* metadata;
    Content content;
} Msg;

#include <jansson.h>

void load_header(const json_t* json, Header* header);
json_t* dump_header(const Header* header);

void load_metadata(const json_t* json, void** metadata);
json_t* dump_metadata(const void* metadata);

void load_content(const json_t* json, MsgType msg_type, Content* content);
json_t* dump_content(MsgType msg_type, const Content* content);

const char* dump_msg_type(MsgType msg_type);

#endif // __IALDOR_MSG_H__
