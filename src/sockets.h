typedef struct Sockets {
    void* ctx;
    void* publish;
    void* requests;
    void* control;
    void* stdin;
    void* heartbeat;
} Sockets;

void init_sockets(Sockets* sockets, Profile* profile);
void free_sockets(Sockets* sockets);
