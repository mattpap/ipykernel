cmake_minimum_required(VERSION 2.8)

project(ipykernel)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -ggdb -Wall")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_library(M_LIB m)
set(LIBS ${LIBS} ${M_LIB})

find_library(M_UUID uuid)
set(LIBS ${LIBS} ${M_UUID})

find_package(Threads REQUIRED)
include_directories(${Threads_INCLUDE_DIRS})
set(LIBS ${LIBS} ${Threads_LIBRARIES})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OPENSSL_LIBRARIES})

find_package(ZMQ REQUIRED)
include_directories(${ZMQ_INCLUDE_DIRS})
set(LIBS ${LIBS} ${ZMQ_LIBRARIES})

find_package(JANSSON REQUIRED)
include_directories(${JANSSON_INCLUDE_DIRS})
set(LIBS ${LIBS} ${JANSSON_LIBRARIES})

add_subdirectory(src)
add_subdirectory(examples)
