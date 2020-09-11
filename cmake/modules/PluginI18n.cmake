#
# Copyright (c) 2020 by Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# --- Translations rules (i18n)

include(${CMAKE_SOURCE_DIR}/i18n/i18n_list.cmake)

set_source_files_properties(${plugin_translation_files} PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/i18n)

foreach(file ${plugin_translation_files})

    if(NOT EXISTS "${file}")

       message(STATUS "Translation file ${file} do not exists yet.")
       message(STATUS "It will be created in ${CMAKE_SOURCE_DIR}/i18n")
       message(STATUS "Don't forget to add this new file on git repository.")

       qt5_create_translation(missing_i18n_QM
                              ${CMAKE_SOURCE_DIR}
                              ${file}
       )

    endif()

endforeach()

qt5_add_translation(plugin_i18n_QM
                    ${plugin_translation_files}
)

foreach(file ${plugin_i18n_QM})

    get_filename_component(directory ${file} DIRECTORY)
    get_filename_component(basename  ${file} NAME)

    set(PLUGIN_QM_XML "${PLUGIN_QM_XML}<file>${directory}/${basename}</file>")

endforeach()

configure_file(${CMAKE_SOURCE_DIR}/cmake/templates/i18n.qrc.in_cmake ${CMAKE_BINARY_DIR}/i18n.qrc)

