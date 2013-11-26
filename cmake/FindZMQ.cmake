# - Try to find ZMQ
# Once done this will define
#
#  ZMQ_FOUND - system has ZMQ
#  ZMQ_INCLUDE_DIRS - the ZMQ include directory
#  ZMQ_LIBRARIES - Link these to use ZMQ
#  ZMQ_DEFINITIONS - Compiler switches required for using ZMQ
#
#  Copyright (c) 2011 Lee Hambley <lee.hambley@gmail.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if (ZMQ_LIBRARIES AND ZMQ_INCLUDE_DIRS)
    set(ZMQ_FOUND TRUE)
else (ZMQ_LIBRARIES AND ZMQ_INCLUDE_DIRS)

    find_path(ZMQ_INCLUDE_DIR
        NAMES
            zmq.h
        PATHS
            ~/.local/include
            /usr/local/include
            /usr/include
        NO_DEFAULT_PATH
    )

    find_library(ZMQ_LIBRARY
        NAMES
            zmq
        PATHS
            ~/.local/lib
            /usr/local/lib
            /usr/lib
        NO_DEFAULT_PATH
    )

    set(ZMQ_INCLUDE_DIRS ${ZMQ_INCLUDE_DIR})

    if (ZMQ_LIBRARY)
        set(ZMQ_LIBRARIES ${ZMQ_LIBRARIES} ${ZMQ_LIBRARY})
    endif (ZMQ_LIBRARY)

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(ZMQ DEFAULT_MSG ZMQ_LIBRARIES ZMQ_INCLUDE_DIRS)

    mark_as_advanced(ZMQ_INCLUDE_DIRS ZMQ_LIBRARIES)
endif (ZMQ_LIBRARIES AND ZMQ_INCLUDE_DIRS)
