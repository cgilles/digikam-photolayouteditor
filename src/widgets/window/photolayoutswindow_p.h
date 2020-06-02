/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2011-09-01
 * Description : a plugin to create photo layouts by fusion of several images.
 *
 * Copyright (C) 2011      by Lukasz Spas <lukasz dot spas at gmail dot com>
 * Copyright (C) 2009-2020 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef PHOTO_LAYOUTS_EDITOR_P_H
#define PHOTO_LAYOUTS_EDITOR_P_H

#include "photolayoutswindow.h"

// Qt includes

#include <QAction>
#include <QFileDialog>
#include <QStyle>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QDockWidget>
#include <QHeaderView>
#include <QLabel>
#include <QApplication>
#include <QPushButton>
#include <QPluginLoader>
#include <QFile>
#include <QPrintPreviewDialog>
#include <QImageWriter>
#include <QPrintDialog>
#include <QDesktopWidget>
#include <QStatusBar>
#include <QMessageBox>
#include <QMenuBar>
#include <QPrinter>
#include <QDebug>
#include <QPointer>
#include <QSettings>

// KDE includes

#include <kactioncollection.h>

// digiKam includes

#include "imagedialog.h"
#include "dmessagebox.h"
#include "digikam_globals.h"

// Local includes

#include "CanvasSize.h"
#include "LayersTree.h"
#include "LayersTreeTitleWidget.h"
#include "ToolsDockWidget.h"
#include "EffectsEditorTool.h"
#include "BorderEditTool.h"
#include "AbstractPhotoEffectFactory.h"
#include "PLEStatusBar.h"
#include "CanvasSizeDialog.h"
#include "Canvas.h"
#include "Scene.h"
#include "LayersSelectionModel.h"
#include "UndoCommandEventFilter.h"
#include "PhotoEffectsLoader.h"
#include "GridSetupDialog.h"
#include "pleconfigdialog.h"
#include "StandardEffectsFactory.h"
#include "StandardBordersFactory.h"
#include "pleglobal.h"
#include "ProgressEvent.h"
#include "BorderDrawerInterface.h"
#include "BorderDrawersLoader.h"
#include "NewCanvasDialog.h"

using namespace Digikam;

// NOTE: Q_*_RESOURCE cannot be used in a namespace

inline void initIconsResource()    { Q_INIT_RESOURCE(icons);    }
inline void cleanupIconsResource() { Q_CLEANUP_RESOURCE(icons); }

namespace PhotoLayoutsEditor
{

class PhotoLayoutsWindow::Private
{
    public:

        Private()
          : centralWidget(nullptr),
            openNewFileAction(nullptr),
            openFileAction(nullptr),
            openRecentFilesMenu(nullptr),
            saveAction(nullptr),
            saveAsAction(nullptr),
            saveAsTemplateAction(nullptr),
            exportFileAction(nullptr),
            printPreviewAction(nullptr),
            printAction(nullptr),
            closeAction(nullptr),
            quitAction(nullptr),
            undoAction(nullptr),
            redoAction(nullptr),
            settingsAction(nullptr),
            addImageAction(nullptr),
            showGridToggleAction(nullptr),
            gridConfigAction(nullptr),
            changeCanvasSizeAction(nullptr),
            tree(nullptr),
            treeWidget(nullptr),
            treeTitle(nullptr),
            toolsWidget(nullptr),
            toolEffects(nullptr),
            toolBorders(nullptr),
            statusBar(nullptr),
            fileDialog(nullptr),
            canvas(nullptr),
            interface(nullptr)
        {
        }

        ~Private()
        {
            Q_DELETE(centralWidget)

            // File menu
            Q_DELETE(openNewFileAction)
            Q_DELETE(openFileAction)
            Q_DELETE(openRecentFilesMenu)
            Q_DELETE(saveAction)
            Q_DELETE(saveAsAction)
            Q_DELETE(saveAsTemplateAction)
            Q_DELETE(exportFileAction)
            Q_DELETE(printPreviewAction)
            Q_DELETE(printAction)
            Q_DELETE(closeAction)
            Q_DELETE(quitAction)

            // Edit menu
            Q_DELETE(undoAction)
            Q_DELETE(redoAction)
            Q_DELETE(settingsAction)

            // View menu
            Q_DELETE(addImageAction)
            Q_DELETE(showGridToggleAction)
            Q_DELETE(gridConfigAction)
            Q_DELETE(changeCanvasSizeAction)

            // Other
            Q_DELETE(tree)
        }

        // Central widget of the window
        QWidget*                                        centralWidget;

        // File menu
        QAction*                                        openNewFileAction;
        QAction*                                        openFileAction;
        KRecentFilesAction*                             openRecentFilesMenu;
        QAction*                                        saveAction;
        QAction*                                        saveAsAction;
        QAction*                                        saveAsTemplateAction;
        QAction*                                        exportFileAction;
        QAction*                                        printPreviewAction;
        QAction*                                        printAction;
        QAction*                                        closeAction;
        QAction*                                        quitAction;

        // Edit menu
        QAction*                                        undoAction;
        QAction*                                        redoAction;
        QAction*                                        settingsAction;

        // Canvas menu
        QAction*                                        addImageAction;
        KToggleAction*                                  showGridToggleAction;
        QAction*                                        gridConfigAction;
        QAction*                                        changeCanvasSizeAction;

        // Tree of layers
        LayersTree*                                     tree;
        QDockWidget*                                    treeWidget;
        LayersTreeTitleWidget*                          treeTitle;

        // Tools
        ToolsDockWidget*                                toolsWidget;
        EffectsEditorTool*                              toolEffects;
        BorderEditTool*                                 toolBorders;

        // Plugins
        QMap<QString, AbstractPhotoEffectFactory*>      effectsMap;
        QMap<QString, BorderDrawerFactoryInterface*>    bordersMap;

        PLEStatusBar *                                  statusBar;

        QFileDialog*                                    fileDialog;

        Canvas*                                         canvas;
        DInfoInterface*                                 interface;
};

// ---------------------------------------------------------------------

class CanvasSizeChangeCommand : public QUndoCommand
{
public:

    CanvasSizeChangeCommand(const CanvasSize& size, Canvas* canvas, QUndoCommand* parent = nullptr)
      : QUndoCommand(QObject::tr("Canvas size change"), parent),
        m_size(size),
        m_canvas(canvas)
    {
    }

    virtual void redo()
    {
        this->run();
    }

    virtual void undo()
    {
        this->run();
    }

    void run()
    {
        CanvasSize temp = m_canvas->canvasSize();
        m_canvas->setCanvasSize(m_size);
        m_size          = temp;
    }

private:

    CanvasSize m_size;
    Canvas*    m_canvas;
};

PhotoLayoutsWindow* PhotoLayoutsWindow::m_instance = nullptr;

} // namespace PhotoLayoutsEditor

#endif // PHOTO_LAYOUTS_EDITOR_P_H
