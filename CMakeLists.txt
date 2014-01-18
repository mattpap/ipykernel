cmake_minimum_required(VERSION 2.8)

project(ipykernel)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -ggdb -Wall")

set(SRCS
    src/options.c
    src/profile.c
    src/sockets.c
    src/heartbeat.c
    src/eventloop.c
    src/communication.c
    src/handlers.c
    src/msg.c
    src/zeromq.c
    src/hmac.c
    src/json.c
    src/uuid.c
    src/concat.c
)

add_library(ipykernel STATIC ${SRCS})
# add_library(ipykernel SHARED ${SRCS})

find_library(M_LIB m)
target_link_libraries(ipykernel ${M_LIB})

find_library(M_UUID uuid)
target_link_libraries(ipykernel ${M_UUID})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(Threads REQUIRED)
if (Threads_FOUND)
    include_directories(${Threads_INCLUDE_DIRS})
    target_link_libraries(ipykernel ${Threads_LIBRARIES})
endif (Threads_FOUND)

find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIRS})
    target_link_libraries(ipykernel ${OPENSSL_LIBRARIES})
endif (OPENSSL_FOUND)

find_package(ZMQ REQUIRED)
if (ZMQ_FOUND)
    include_directories(${ZMQ_INCLUDE_DIRS})
    target_link_libraries(ipykernel ${ZMQ_LIBRARIES})
endif (ZMQ_FOUND)

find_package(JANSSON REQUIRED)
if (JANSSON_FOUND)
    include_directories(${JANSSON_INCLUDE_DIRS})
    target_link_libraries(ipykernel ${JANSSON_LIBRARIES})
endif (JANSSON_FOUND)
