#ifndef __IALDOR_COMMUNICATION_H__
#define __IALDOR_COMMUNICATION_H__

#include "msg.h"

void msg_recv(void* socket, Msg* msg);
void msg_send(void* socket, Msg* msg);

void send_reply(void* socket, const Msg* msg, MsgType msg_type, const Content* content);
void publish_reply(const Msg* msg, MsgType msg_type, const Content* content);

void send_status(ExecutionState state);
void send_ok(Msg* msg, int execution_count);
void send_error(Msg* msg, int execution_count, char* error);
void send_abort(Msg* msg, int execution_count);
void send_stream(Msg* msg, char* name, char* data);
void send_stdin(Msg* msg, char* prompt);

#endif // __IALDOR_COMMUNICATION_H__
