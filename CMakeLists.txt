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

#find_package(ECM "1.7.0" CONFIG REQUIRED)
#set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
#include(KDEFrameworkCompilerSettings)
#include(KDECompilerSettings)

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

    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/AbstractMovableModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/LayersModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/LayersModelItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/LayersSelectionModel.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/BorderChangeListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/BorderDrawerInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/BorderDrawersLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/BordersGroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/tools/StandardBordersFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/tools/PolaroidBorderDrawer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/tools/SolidBorderDrawer.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/AbstractPhotoEffectInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/PhotoEffectChangeListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/PhotoEffectsGroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/PhotoEffectsLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/StandardEffectsFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/BlurPhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/ColorizePhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/GrayscalePhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/SepiaPhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/tools/NegativePhotoEffect.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/AbstractPhotoItemLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/CanvasLoadingThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/CanvasSavingThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/ImageLoadingThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/PhotoItemLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/SceneBackgroundLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/SceneBorderLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/TextItemLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/ProgressEvent.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/pleglobal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qeditfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtbuttonpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qteditorfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtgroupboxpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertybrowserutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertymanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qttreepropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtvariantproperty.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/listeners/TextColorChangeListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/listeners/TextFontChangeListener.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/UndoMoveRowsCommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/UndoRemoveItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/UndoCommandEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/UndoCommandEventFilter.cpp


    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/LayersTree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/LayersTreeDelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/LayersTreeMenu.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/AbstractItemsListViewTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/AbstractItemsTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/AbstractTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/BorderEditTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/CanvasEditTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/EffectsEditorTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/TextEditorTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/ToolsDockWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/ZoomTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/PatternsComboBox.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/CanvasSizeDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/GridSetupDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/NewCanvasDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/CanvasSizeWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/TemplatesModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/TemplatesView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/pleconfigdialog.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/window/photolayoutswindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/window/plestatusbar.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items/AbstractPhoto.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items/AbstractPhoto_p.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items/TextItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items/AbstractItemInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items/PhotoItem.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/Canvas.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/CanvasSize.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/CropWidgetItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/MousePressListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/RotationWidgetItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/ScalingWidgetItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/SceneBackground.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/SceneBorder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas/Scene.cpp
)

qt5_add_resources(photolayoutseditorcore_SRCS
                  ${CMAKE_CURRENT_SOURCE_DIR}/res/icons.qrc
                  ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertybrowser.qrc
)

qt5_wrap_ui(photolayoutseditorcore_SRCS
            ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/window/photolayoutswindow.ui
)

add_library(photolayoutseditorcore STATIC ${photolayoutseditorcore_SRCS})

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
