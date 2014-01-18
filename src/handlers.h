#ifndef __IPYKERNEL_HANDLERS_H__
#define __IPYKERNEL_HANDLERS_H__

#include "msg.h"

void execute_handler(void* socket, Msg* msg);
void complete_handler(void* socket, Msg* msg);
void kernel_info_handler(void* socket, Msg* msg);
void connect_handler(void* socket, Msg* msg);
void shutdown_handler(void* socket, Msg* msg);
void object_info_handler(void* socket, Msg* msg);
void history_handler(void* socket, Msg* msg);

#endif // __IPYKERNEL_HANDLERS_H__
