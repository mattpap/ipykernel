cmake_minimum_required(VERSION 2.8)

project(IAldor)

if (NOT ALDOR)
    message(FATAL_ERROR "Aldor directory not specified. Use cmake -DALDOR=some/path .")
endif ()

set(ALDOR_SRC ${ALDOR}/aldor/aldor/src)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb")

link_directories(${ALDOR_SRC})

add_executable(ialdor src/ialdor.c
                      src/concat.c
                      ${ALDOR_SRC}/aldor-axlcomp.o
                      ${ALDOR_SRC}/aldor-cmdline.o)

target_link_libraries(ialdor libphase.a libstruct.a libgen.a libport.a)

find_library(M_LIB m)
target_link_libraries(ialdor ${M_LIB})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(Threads REQUIRED)
if (Threads_FOUND)
    include_directories(${Threads_INCLUDE_DIRS})
    target_link_libraries(ialdor ${Threads_LIBRARIES})
endif (Threads_FOUND)

find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    include_directories(${OpenSSL_INCLUDE_DIRS})
    target_link_libraries(ialdor ${OpenSSL_LIBRARIES})
endif (OpenSSL_FOUND)

find_package(ZMQ REQUIRED)
if (ZMQ_FOUND)
    include_directories(${ZMQ_INCLUDE_DIRS})
    target_link_libraries(ialdor ${ZMQ_LIBRARIES})
endif (ZMQ_FOUND)

find_package(JANSSON REQUIRED)
if (JANSSON_FOUND)
    include_directories(${JANSSON_INCLUDE_DIRS})
    target_link_libraries(ialdor ${JANSSON_LIBRARIES})
endif (JANSSON_FOUND)
