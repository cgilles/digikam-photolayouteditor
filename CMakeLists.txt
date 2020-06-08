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
)

find_package(Threads)

find_package(DigikamCore
             CONFIG REQUIRED)

set_package_properties(DigikamCore PROPERTIES
                       URL "http://www.digikam.org"
                       DESCRIPTION "digiKam core library"
)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/BorderChangeListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/BorderDrawerInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/borderdrawersloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/BordersGroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/tools/StandardBordersFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/tools/PolaroidBorderDrawer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/tools/SolidBorderDrawer.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/abstractphotoeffectfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/abstractphotoeffectinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/PhotoEffectChangeListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/PhotoEffectsGroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/PhotoEffectsLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/StandardEffectsFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/BlurPhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/ColorizePhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/GrayscalePhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/SepiaPhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/NegativePhotoEffect.cpp

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

    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/UndoMoveRowsCommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/UndoRemoveItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/UndoCommandEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/UndoCommandEventFilter.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/LayersTree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/LayersTreeDelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/LayersTreeMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/LayersModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/LayersModelItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/LayersSelectionModel.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/AbstractItemsListViewTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/AbstractItemsTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/AbstractTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/abstractmovablemodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/BorderEditTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/plecanvasedittool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/EffectsEditorTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/TextEditorTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/ToolsDockWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/ZoomTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/PatternsComboBox.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/plecanvassizedialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/plecanvasnewdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/plecanvassizewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/pleconfigdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/GridSetupDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/TemplatesModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/TemplatesView.cpp

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
)

qt5_add_resources(photolayoutseditorcore_SRCS
                  ${CMAKE_CURRENT_SOURCE_DIR}/res/icons.qrc
                  ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertybrowser.qrc
)

qt5_wrap_ui(photolayoutseditorcore_SRCS
            ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/window/plewindow.ui
)

add_library(photolayoutseditorcore STATIC ${photolayoutseditorcore_SRCS})

target_compile_options(photolayoutseditorcore PRIVATE
                       -Wall
                       -Wextra
                       -Wsuggest-override
                       -Woverloaded-virtual
                       -Wzero-as-null-pointer-constant
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
)

install(TARGETS photolayoutseditor RUNTIME DESTINATION bin)

if (ENABLE_DPLUGIN)
    add_subdirectory(dplugin)
endif()

MACRO_ADD_UNINSTALL_TARGET()
