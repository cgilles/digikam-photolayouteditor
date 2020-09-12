/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2011-09-01
 * Description : a plugin to create photo layouts by fusion of several images.
 *
 * Copyright (C) 2011      by Lukasz Spas <lukasz dot spas at gmail dot com>
 * Copyright (C) 2011-2020 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef PLE_WINDOW_P_H
#define PLE_WINDOW_P_H

#include "plewindow.h"

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
#include <QTranslator>
#include <QLocale>

// digiKam includes

#include "imagedialog.h"
#include "dmessagebox.h"
#include "digikam_globals.h"
#include "dpluginaboutdlg.h"

// Local includes

#include "plecanvassize.h"
#include "layerstree.h"
#include "layerstreetitlewidget.h"
#include "toolsdockwidget.h"
#include "effectseditortool.h"
#include "borderedittool.h"
#include "abstractphotoeffectfactory.h"
#include "plestatusbar.h"
#include "plecanvassizedialog.h"
#include "plecanvas.h"
#include "plescene.h"
#include "layersselectionmodel.h"
#include "undocommandeventfilter.h"
#include "photoeffectsloader.h"
#include "gridsetupdialog.h"
#include "pleconfigdialog.h"
#include "standardeffectsfactory.h"
#include "standardbordersfactory.h"
#include "pleglobal.h"
#include "progressevent.h"
#include "borderdrawerinterface.h"
#include "borderdrawersloader.h"
#include "plecanvasnewdialog.h"
#include "ui_plewindow.h"

using namespace Digikam;

namespace PhotoLayoutsEditor
{

class PLEWindow::Private
{
    public:

        explicit Private()
          : centralWidget(nullptr),
            tree(nullptr),
            treeWidget(nullptr),
            treeTitle(nullptr),
            toolsWidget(nullptr),
            toolEffects(nullptr),
            toolBorders(nullptr),
            statusBar(nullptr),
            fileDialog(nullptr),
            canvas(nullptr),
            interface(nullptr),
            ui(nullptr),
            plugin(nullptr)
        {
        }

        ~Private()
        {
            delete centralWidget;
            delete tree;
        }

        // Central widget of the window
        QWidget*                                        centralWidget;

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

        PLEStatusBar*                                   statusBar;

        QFileDialog*                                    fileDialog;

        PLECanvas*                                         canvas;
        DInfoInterface*                                 interface;
        Ui::PLEWindow*                                  ui;
        DPluginGeneric*                                 plugin;
};

// ---------------------------------------------------------------------

class PLECanvasSizeChangeCommand : public QUndoCommand
{
public:

    PLECanvasSizeChangeCommand(const PLECanvasSize& size, PLECanvas* canvas, QUndoCommand* parent = nullptr)
      : QUndoCommand(QObject::tr("PLECanvas size change"), parent),
        m_size(size),
        m_canvas(canvas)
    {
    }

    virtual void redo() override
    {
        this->run();
    }

    virtual void undo() override
    {
        this->run();
    }

    void run()
    {
        PLECanvasSize temp = m_canvas->canvasSize();
        m_canvas->setPLECanvasSize(m_size);
        m_size          = temp;
    }

private:

    PLECanvasSize m_size;
    PLECanvas*    m_canvas;
};

static PLEWindow* m_instance = nullptr;

} // namespace PhotoLayoutsEditor

#endif // PLE_WINDOW_P_H
