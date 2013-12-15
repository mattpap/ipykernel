#ifndef __IALDOR_SOCKETS_H__
#define __IALDOR_SOCKETS_H__

typedef struct Sockets {
    void* ctx;
    void* publish;
    void* requests;
    void* control;
    void* stdin;
    void* heartbeat;
} Sockets;

void init_sockets(Sockets* sockets);
void free_sockets(Sockets* sockets);

#endif // __IALDOR_SOCKETS_H__
