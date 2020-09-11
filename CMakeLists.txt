#
# Copyright (c) 2010-2020, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

project(PhotoLayoutsEditor)

cmake_minimum_required(VERSION "3.1.0")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

option(ENABLE_DPLUGIN "Build digiKam plugin version of Photo Layouts Editor tool (default=OFF)" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

include(MacroInstall)
include(FeatureSummary)

find_package(Qt5 "5.6.0" REQUIRED
             NO_MODULE COMPONENTS
             Core
             Widgets
             Gui
             Svg
             Xml
             PrintSupport
             LinguistTools              # for i18n
)

find_package(Threads)

find_package(DigikamCore
             CONFIG REQUIRED)

set_package_properties(DigikamCore PROPERTIES
                       URL "http://www.digikam.org"
                       DESCRIPTION "digiKam core library"
)

include(PluginI18n)

include_directories($<TARGET_PROPERTY:Digikam::digikamcore,INTERFACE_INCLUDE_DIRECTORIES>/digikam
                    $<TARGET_PROPERTY:Qt5::Xml,INTERFACE_INCLUDE_DIRECTORIES>
                    $<TARGET_PROPERTY:Qt5::Svg,INTERFACE_INCLUDE_DIRECTORIES>
                    $<TARGET_PROPERTY:Qt5::Gui,INTERFACE_INCLUDE_DIRECTORIES>
                    $<TARGET_PROPERTY:Qt5::Core,INTERFACE_INCLUDE_DIRECTORIES>
                    $<TARGET_PROPERTY:Qt5::Widgets,INTERFACE_INCLUDE_DIRECTORIES>
                    $<TARGET_PROPERTY:Qt5::PrintSupport,INTERFACE_INCLUDE_DIRECTORIES>

                    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/window
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/settings
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/listeners
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/tools

                    ${CMAKE_BINARY_DIR}/
)

add_subdirectory(data)

set(photolayoutseditorcore_SRCS

    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/borderdrawerfactoryinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/borderchangelistener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/borderdrawerinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/borderdrawersloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/bordersgroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/tools/standardbordersfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/tools/polaroidborderdrawer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/tools/solidborderdrawer.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/abstractphotoeffectfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/abstractphotoeffectinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/photoeffectchangelistener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/photoeffectsgroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/photoeffectsloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/standardeffectsfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/blurphotoeffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/colorizephotoeffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/grayscalephotoeffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/sepiaphotoeffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/negativephotoeffect.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/plecanvasloadingthread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/plecanvassavingthread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/plescenebackgroundloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/plesceneborderloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/abstractphotoitemloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/imageloadingthread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/photoitemloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/textitemloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/progressevent.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/pleglobal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/pleeditfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtbuttonpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qteditorfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtgroupboxpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertybrowserutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertymanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qttreepropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtvariantproperty.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/undomoverowscommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/undoremoveitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/undocommandevent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/undocommandeventfilter.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/layerstree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/layerstreedelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/layerstreemenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/layersmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/layersmodelitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/layersselectionmodel.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/abstractitemslistviewtool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/abstractitemstool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/abstracttool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/abstractmovablemodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/borderedittool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/plecanvasedittool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/effectseditortool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/texteditortool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/toolsdockwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/zoomtool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/patternscombobox.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/plecanvassizedialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/plecanvasnewdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/plecanvassizewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/pleconfigdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/gridsetupdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/templatesmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/templatesview.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/window/plewindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/window/plestatusbar.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items/textcolorchangelistener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items/textfontchangelistener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items/textitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items/abstractphoto.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items/abstractphoto_p.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items/abstractiteminterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items/photoitem.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/plecanvas.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/plecanvassize.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/plescenebackground.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/plesceneborder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/plescene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/cropwidgetitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/mousepresslistener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/rotationwidgetitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/scalingwidgetitem.cpp
    ${plugin_i18n_QM}
)

qt5_add_resources(photolayoutseditorcore_SRCS
                  ${CMAKE_CURRENT_SOURCE_DIR}/res/icons.qrc
                  ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertybrowser.qrc
)

qt5_wrap_ui(photolayoutseditorcore_SRCS
            ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/window/plewindow.ui
)

# Enforce modern Qt code
add_definitions(-DQT_DEPRECATED_WARNINGS
                -DQT_USE_QSTRINGBUILDER
                -DQT_NO_CAST_TO_ASCII
                -DQT_NO_CAST_FROM_ASCII
                -DQT_NO_CAST_FROM_BYTEARRAY
                -DQT_NO_URL_CAST_FROM_STRING
                -DQT_STRICT_ITERATORS
                -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
                -DQT_DEPRECATED_WARNINGS_SINCE=0x060000
                -DQT_DISABLE_DEPRECATED_BEFORE=0x050E00
#                -DQT_NO_FOREACH
#                -DQT_NO_KEYWORDS
)

add_library(photolayoutseditorcore STATIC ${photolayoutseditorcore_SRCS})

target_compile_options(photolayoutseditorcore PRIVATE
                       -Waddress
                       -Wall
                       -Werror
                       -Wextra
                       -Winit-self
                       -Wfloat-conversion
                       -Wnull-dereference
                       -Wsuggest-override
                       -Wpedantic
                       -Woverloaded-virtual
                       -Wunused-value
                       -Wzero-as-null-pointer-constant
                       -Wno-conversion
                       -Wno-deprecated-copy
                       -Wno-int-in-bool-context
)

if(NOT MSVC)
    target_compile_options(photolayoutseditorcore PRIVATE -fPIC)
endif()

# =======================================================
# Stand-alone version of PhotoLayoutsEditor

add_executable(photolayoutseditor
               ${CMAKE_CURRENT_SOURCE_DIR}/main/main.cpp
)

target_link_libraries(photolayoutseditor

                      photolayoutseditorcore

                      Digikam::digikamcore

                      Qt5::Gui
                      Qt5::Xml
                      Qt5::Svg
                      Qt5::PrintSupport

                      ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS photolayoutseditor RUNTIME DESTINATION bin)

if (ENABLE_DPLUGIN)
    add_subdirectory(dplugin)
endif()

MACRO_ADD_UNINSTALL_TARGET()
