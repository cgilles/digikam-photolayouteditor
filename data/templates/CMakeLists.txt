#
# Copyright (c) 2010-2020, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

file(GLOB templates_dir
     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*
     LIST_DIRECTORIES true)

foreach(child ${templates_dir})

    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})

        install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child}
                DESTINATION ${DATA_INSTALL_DIR}/photolayoutseditor)

    endif()

endforeach()

