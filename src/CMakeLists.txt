set(SRCS
    options.c
    profile.c
    sockets.c
    heartbeat.c
    eventloop.c
    communication.c
    interpreter.c
    handlers.c
    msg.c
    zeromq.c
    hmac.c
    json.c
    uuid.c
    concat.c
)

add_library(ipykernel STATIC ${SRCS})
