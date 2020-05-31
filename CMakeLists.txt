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

find_package(ECM "1.7.0" CONFIG REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH})

include(MacroInstall)
include(FeatureSummary)

include(ECMSetupVersion)

include(KDEInstallDirs)
#include(KDECMakeSettings)
#include(KDEFrameworkCompilerSettings)
#include(KDECompilerSettings)

find_package(Qt5 "5.6.0" REQUIRED
             NO_MODULE COMPONENTS
             Core
             Widgets
             Gui
             Concurrent
             Svg
             Xml
             PrintSupport
)

find_package(KF5 "5.5.0" REQUIRED
             COMPONENTS
             XmlGui
             Config
             Service
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

                    $<TARGET_PROPERTY:KF5::XmlGui,INTERFACE_INCLUDE_DIRECTORIES>
                    $<TARGET_PROPERTY:KF5::ConfigCore,INTERFACE_INCLUDE_DIRECTORIES>
                    $<TARGET_PROPERTY:KF5::Service,INTERFACE_INCLUDE_DIRECTORIES>

                    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/canvas
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/extra
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/items
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/menus
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/window
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/settings
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/listeners
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/events
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/effectplugins
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/borderplugins

                    ${CMAKE_BINARY_DIR}/
)

add_subdirectory(data)

set(photolayoutseditorcore_SRCS

    ${CMAKE_CURRENT_SOURCE_DIR}/src/events/ProgressEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/events/UndoCommandEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/events/UndoCommandEventFilter.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/AbstractMovableModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/LayersModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/LayersModelItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/LayersSelectionModel.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/borderplugins/StandardBordersFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borderplugins/PolaroidBorderDrawer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borderplugins/SolidBorderDrawer.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/BorderChangeListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/BorderDrawerInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/BorderDrawersLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/BordersGroup.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/effectplugins/StandardEffectsFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effectplugins/BlurPhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effectplugins/ColorizePhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effectplugins/GrayscalePhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effectplugins/SepiaPhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effectplugins/NegativePhotoEffect.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/AbstractPhotoEffectInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/PhotoEffectChangeListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/PhotoEffectsGroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/PhotoEffectsLoader.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/AbstractPhotoItemLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/CanvasLoadingThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/CanvasSavingThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/ImageLoadingThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/PhotoItemLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/SceneBackgroundLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/SceneBorderLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/threads/TextItemLoader.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/global.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/KEditFactory.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtbuttonpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qteditorfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtgroupboxpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertybrowserutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertymanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qttreepropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtvariantproperty.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/extra/PatternsComboBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/extra/PLEStatusBar.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/LayersTree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/layers/LayersTreeDelegate.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/menus/LayersTreeMenu.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/AbstractItemsListViewTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/AbstractItemsTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/AbstractTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/BorderEditTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/CanvasEditTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/EffectsEditorTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/TextEditorTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/ToolsDockWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tools/ZoomTool.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/CanvasSizeDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/GridSetupDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/NewCanvasDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/CanvasSizeWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/TemplatesModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dialogs/TemplatesView.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/listeners/TextColorChangeListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/listeners/TextFontChangeListener.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/undo/UndoMoveRowsCommand.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/settings/PLEConfigDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/settings/PLEConfigViewWidget.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/window/photolayoutswindow.cpp

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

set(photolayoutseditorcore_RCS
    ${CMAKE_CURRENT_SOURCE_DIR}/res/icons.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extra/qtpropertybrowser/qtpropertybrowser.qrc
   )

qt5_add_resources(photolayoutseditorcore_SRCS ${photolayoutseditorcore_RCS})

kconfig_add_kcfg_files(photolayoutseditorcore_SRCS GENERATE_MOC ${CMAKE_CURRENT_SOURCE_DIR}/src/settings/PLEConfigSkeleton.kcfgc)

add_library(photolayoutseditorcore STATIC ${photolayoutseditorcore_SRCS})

if(NOT MSVC)
    target_compile_options(photolayoutseditorcore PRIVATE -fPIC)
endif()

install(FILES    ${CMAKE_CURRENT_SOURCE_DIR}/src/settings/photolayoutseditor.kcfg DESTINATION ${KCFG_INSTALL_DIR})
#install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/window/photolayoutseditor.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES    ${CMAKE_CURRENT_SOURCE_DIR}/src/effects/photolayoutseditoreffectplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES    ${CMAKE_CURRENT_SOURCE_DIR}/src/borders/photolayoutseditorborderplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

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

                      KF5::XmlGui
                      KF5::ConfigCore
                      KF5::Service
)

install(TARGETS photolayoutseditor RUNTIME DESTINATION bin)

if (ENABLE_DPLUGIN)
    add_subdirectory(dplugin)
endif()

MACRO_ADD_UNINSTALL_TARGET()
