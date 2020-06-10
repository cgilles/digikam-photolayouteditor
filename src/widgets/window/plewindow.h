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

#ifndef PLE_WINDOW_H
#define PLE_WINDOW_H

// Qt includes

#include <QUndoStack>
#include <QDebug>
#include <QUrl>
#include <QMainWindow>

// digiKam includes

#include "dinfointerface.h"
#include "dplugingeneric.h"

using namespace Digikam;

namespace PhotoLayoutsEditor
{

class PLECanvas;
class PLECanvasSize;
class PLECanvasSizeChangeCommand;
class ProgressEvent;
class UndoCommandEventFilter;

class PLEWindow : public QMainWindow
{
    Q_OBJECT

public:

    ~PLEWindow();
    static PLEWindow* instance(DPluginGeneric* const plugin = nullptr);

    void addUndoCommand(QUndoCommand* const command);
    void beginUndoCommandGroup(const QString& name);
    void endUndoCommandGroup();

    void setInterface(DInfoInterface* const interface);
    DInfoInterface* interface() const;
    bool hasInterface()         const;

    void loadImages(const QList<QUrl>& urls);

public Q_SLOTS:

    void openFile();
    void openFileDialog();
    void openFile(const QUrl& fileUrl);
    void saveFile();
    void saveFile(const QUrl& fileUrl, bool setFileAsDefault = true);
    void saveAsFile();
    void saveAsTemplate();
    void exportFile();
    void printPreview();
    void print();
    bool closeDocument();
    void loadNewImage();
    void setGridVisible(bool isVisible);
    void createPLECanvas(const PLECanvasSize& size);
    void createPLECanvas(const QUrl& fileUrl);
    void settings();
    void setupGrid();
    void changePLECanvasSize();
    void setTemplateEditMode(bool isEnabled);

protected:

    void closeEvent(QCloseEvent*) override;
    void progressEvent(ProgressEvent* event);

private Q_SLOTS:

    void refreshActions();
    void slotAbout();

private:

    explicit PLEWindow(DPluginGeneric* const plugin = nullptr);

    void setupActions();
    void createWidgets();
    void loadEffects();
    void loadBorders();
    void prepareSignalsConnections();

private:

    class Private;
    Private* const d;

    friend class UndoCommandEventFilter;
};

} // namespace PhotoLayoutsEditor

#endif // PLE_WINDOW_H
