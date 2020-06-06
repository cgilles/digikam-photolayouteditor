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

#ifndef PHOTO_LAYOUTS_WINDOW_H
#define PHOTO_LAYOUTS_WINDOW_H

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

class Canvas;
class CanvasSize;
class CanvasSizeChangeCommand;
class ProgressEvent;
class UndoCommandEventFilter;

class PhotoLayoutsWindow : public QMainWindow
{
    Q_OBJECT

public:

    ~PhotoLayoutsWindow();
    static PhotoLayoutsWindow* instance(DPluginGeneric* const plugin = nullptr);

    void addUndoCommand(QUndoCommand* const command);
    void beginUndoCommandGroup(const QString& name);
    void endUndoCommandGroup();

    void setInterface(DInfoInterface* const interface);
    DInfoInterface* interface() const;
    bool hasInterface()         const;

    void loadImages(const QList<QUrl>& urls);

public Q_SLOTS:

    void open();
    void openDialog();
    void open(const QUrl& fileUrl);
    void save();
    void saveAs();
    void saveAsTemplate();
    void saveFile(const QUrl& fileUrl = QUrl(), bool setFileAsDefault = true);
    void exportFile();
    void printPreview();
    void print();
    bool closeDocument();
    void loadNewImage();
    void setGridVisible(bool isVisible);
    void createCanvas(const CanvasSize& size);
    void createCanvas(const QUrl& fileUrl);
    void settings();
    void setupGrid();
    void changeCanvasSize();
    void setTemplateEditMode(bool isEnabled);

protected:

    void progressEvent(ProgressEvent* event);

protected Q_SLOTS:

    bool queryClose();
    void refreshActions();
    void slotAbout();

private:

    explicit PhotoLayoutsWindow(DPluginGeneric* const plugin = nullptr);

    void setupActions();
    void createWidgets();
    void loadEffects();
    void loadBorders();
    void prepareSignalsConnections();

private:

    static PhotoLayoutsWindow* m_instance;

    class Private;
    Private* const d;

    friend class UndoCommandEventFilter;
};

} // namespace PhotoLayoutsEditor

#endif // PHOTO_LAYOUTS_WINDOW_H
