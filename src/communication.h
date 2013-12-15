#ifndef __IALDOR_COMMUNICATION_H__
#define __IALDOR_COMMUNICATION_H__

#include "msg.h"

void recv(void* socket, Msg* msg);
void send(void* socket, Msg* msg);

void send_status(ExecutionState state);
void send_ok(Msg* msg, int execution_count);
void send_error(Msg* msg, int execution_count, char* error);
void send_abort(Msg* msg, int execution_count);
void send_stream(Msg* msg, char* name, char* data);

#endif // __IALDOR_COMMUNICATION_H__
