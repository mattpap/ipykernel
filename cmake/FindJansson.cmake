# - Try to find Jansson
# Once done this will define
#
#  JANSSON_FOUND - system has Jansson
#  JANSSON_INCLUDE_DIRS - the Jansson include directory
#  JANSSON_LIBRARIES - Link these to use Jansson
#
#  Copyright (c) 2011 Lee Hambley <lee.hambley@gmail.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if (JANSSON_LIBRARIES AND JANSSON_INCLUDE_DIRS)
    set(JANSSON_FOUND TRUE)
else (JANSSON_LIBRARIES AND JANSSON_INCLUDE_DIRS)
    find_path(JANSSON_INCLUDE_DIR
        NAMES
            jansson.h
        PATHS
            ~/.local/include
            /usr/local/include
            /usr/include
        NO_DEFAULT_PATH
    )

    find_library(JANSSON_LIBRARY
        NAMES
            jansson
        PATHS
            ~/.local/lib
            /usr/local/lib
            /usr/lib
        NO_DEFAULT_PATH
    )

set(JANSSON_INCLUDE_DIRS ${JANSSON_INCLUDE_DIR})

if (JANSSON_LIBRARY)
    set(JANSSON_LIBRARIES ${JANSSON_LIBRARIES} ${JANSSON_LIBRARY})
endif (JANSSON_LIBRARY)

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(Jansson DEFAULT_MSG JANSSON_LIBRARIES JANSSON_INCLUDE_DIRS)

    mark_as_advanced(JANSSON_INCLUDE_DIRS JANSSON_LIBRARIES)
endif (JANSSON_LIBRARIES AND JANSSON_INCLUDE_DIRS)
