#
# Copyright (c) 2020, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(Generic_PhotoLayoutsEditor_Plugin
            MODULE ${CMAKE_CURRENT_SOURCE_DIR}/photolayoutseditorgenericplugin.cpp
)

set_target_properties(Generic_PhotoLayoutsEditor_Plugin PROPERTIES PREFIX "")

target_link_libraries(Generic_PhotoLayoutsEditor_Plugin

                      photolayoutseditorcore

                      Digikam::digikamcore

                      Qt5::Gui
                      Qt5::Xml
                      Qt5::Svg
                      Qt5::PrintSupport

                      KF5::XmlGui
                      KF5::ConfigCore
                      KF5::Service
)

MACRO_ADD_PLUGIN_INSTALL_TARGET(Generic_PhotoLayoutsEditor_Plugin generic)

# ---

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
