#
# Copyright (c) 2010-2020, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

project(PhotoLayout)

cmake_minimum_required(VERSION "3.1.0")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
include(MacroInstall)
include(FeatureSummary)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

option(ENABLE_DPLUGIN "Build digiKam plugin version of Image Mosaic Wall tool (default=OFF)" OFF)

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
                    ${CMAKE_CURRENT_SOURCE_DIR}/extra
                    ${CMAKE_CURRENT_SOURCE_DIR}/threads
                    ${CMAKE_CURRENT_SOURCE_DIR}/undo
                    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/canvas
                    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/dialogs
                    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/extra
                    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/items
                    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/layers
                    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/menus
                    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/tools
                    ${CMAKE_CURRENT_SOURCE_DIR}/settings
                    ${CMAKE_CURRENT_SOURCE_DIR}/models
                    ${CMAKE_CURRENT_SOURCE_DIR}/main
                    ${CMAKE_CURRENT_SOURCE_DIR}/listeners
                    ${CMAKE_CURRENT_SOURCE_DIR}/events
                    ${CMAKE_CURRENT_SOURCE_DIR}/effects
                    ${CMAKE_CURRENT_SOURCE_DIR}/effectplugins
                    ${CMAKE_CURRENT_SOURCE_DIR}/borders
                    ${CMAKE_CURRENT_SOURCE_DIR}/borderplugins
)

add_subdirectory(data)

set(photolayoutseditor_SRCS

    ${CMAKE_CURRENT_SOURCE_DIR}/events/ProgressEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/events/UndoCommandEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/events/UndoCommandEventFilter.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/models/AbstractMovableModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/LayersModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/LayersModelItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/LayersSelectionModel.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/borderplugins/StandardBordersFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/borderplugins/PolaroidBorderDrawer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/borderplugins/SolidBorderDrawer.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/borders/BorderChangeListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/borders/BorderDrawerInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/borders/BorderDrawersLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/borders/BordersGroup.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/effectplugins/StandardEffectsFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/effectplugins/BlurPhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/effectplugins/ColorizePhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/effectplugins/GrayscalePhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/effectplugins/SepiaPhotoEffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/effectplugins/NegativePhotoEffect.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/effects/AbstractPhotoEffectInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/effects/PhotoEffectChangeListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/effects/PhotoEffectsGroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/effects/PhotoEffectsLoader.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/threads/AbstractPhotoItemLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/threads/CanvasLoadingThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/threads/CanvasSavingThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/threads/ImageLoadingThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/threads/PhotoItemLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/threads/SceneBackgroundLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/threads/SceneBorderLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/threads/TextItemLoader.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/extra/global.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extra/KEditFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extra/qtpropertybrowser/qtbuttonpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extra/qtpropertybrowser/qteditorfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extra/qtpropertybrowser/qtgroupboxpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extra/qtpropertybrowser/qtpropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extra/qtpropertybrowser/qtpropertybrowserutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extra/qtpropertybrowser/qtpropertymanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extra/qtpropertybrowser/qttreepropertybrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extra/qtpropertybrowser/qtvariantproperty.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/extra/PatternsComboBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/extra/PLEStatusBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/layers/LayersTree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/layers/LayersTreeDelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/items/AbstractPhoto.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/items/AbstractPhoto_p.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/items/TextItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/items/AbstractItemInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/items/PhotoItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/menus/LayersTreeMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/tools/AbstractItemsListViewTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/tools/AbstractItemsTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/tools/AbstractTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/tools/BorderEditTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/tools/CanvasEditTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/tools/EffectsEditorTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/tools/TextEditorTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/tools/ToolsDockWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/tools/ZoomTool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/dialogs/CanvasSizeDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/dialogs/GridSetupDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/dialogs/NewCanvasDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/dialogs/CanvasSizeWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/dialogs/TemplatesModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/dialogs/TemplatesView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/canvas/Canvas.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/canvas/CanvasSize.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/canvas/CropWidgetItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/canvas/MousePressListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/canvas/RotationWidgetItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/canvas/ScalingWidgetItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/canvas/SceneBackground.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/canvas/SceneBorder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/canvas/Scene.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/listeners/TextColorChangeListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/listeners/TextFontChangeListener.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/undo/UndoMoveRowsCommand.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/settings/PLEConfigDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/PLEConfigViewWidget.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/main/photolayoutswindow.cpp
)

set(photolayoutseditor_RCS
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/extra/qtpropertybrowser/qtpropertybrowser.qrc
   )

qt5_add_resources(photolayoutseditor_SRCS ${photolayoutseditor_RCS})

#kconfig_add_kcfg_files(photolayoutseditor_SRCS GENERATE_MOC settings/PLEConfigSkeleton.kcfgc)

add_library(photolayoutseditor STATIC ${photolayoutseditor_SRCS})

if(NOT MSVC)
    target_compile_options(photolayoutseditor PRIVATE -fPIC)
endif()

#install(FILES settings/photolayoutseditor.kcfg DESTINATION ${KCFG_INSTALL_DIR})
#install(PROGRAMS main/photolayoutseditor.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
#install(FILES main/photolayoutseditorui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/photolayoutseditor)
#install(FILES effects/photolayoutseditoreffectplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
#install(FILES borders/photolayoutseditorborderplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

# =======================================================
# Stand-alone version of PhotoLayoutsEditor

#file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/./resources/app/*photolayoutseditor.png")
#ecm_add_app_icon(photolayoutseditor_SRCS ICONS ${ICONS_SRCS})

add_executable(collage
               ${CMAKE_CURRENT_SOURCE_DIR}/main/collage.cpp
)

target_link_libraries(collage

                      photolayoutseditor
                      digikamcore

                      Qt5::Gui
                      Qt5::Xml
                      Qt5::Svg
)
