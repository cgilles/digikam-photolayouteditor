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

#ifndef PLE_CANVAS_H
#define PLE_CANVAS_H

// Qt includes

#include <QObject>
#include <QSizeF>
#include <QRectF>
#include <QGraphicsScene>
#include <QItemSelection>
#include <QUndoStack>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QUrl>

// Local includes

#include "pleglobal.h"
#include "plecanvassize.h"

namespace PhotoLayoutsEditor
{

class PLECanvasPrivate;
class PLECanvasSavingThread;
class PLEScene;
class LayersModel;
class LayersSelectionModel;
class AbstractPhoto;
class ProgressEvent;

class PLECanvas : public QGraphicsView
{
    Q_OBJECT

public:

    enum SelectionMode
    {
        Viewing        = 1,
        Zooming        = 2,
        MultiSelecting = 4,
        SingleSelcting = 8
    };

public:

    explicit PLECanvas(const PLECanvasSize& size, QWidget* parent = nullptr);
    ~PLECanvas();

    virtual void wheelEvent(QWheelEvent* event) override;

    QDomDocument toSvg() const;
    static PLECanvas* fromSvg(QDomDocument& document);

    void scale(qreal factor, const QPoint& center = QPoint());
    void scale(const QRect& rect);

    /// Hold URL to the file connected with this canvas.
    Q_PROPERTY(QUrl m_file READ file WRITE setFile)
    QUrl file() const;
    void setFile(const QUrl& file);

    /// Saves canvas state to SVG format file
    void save(const QUrl& file, bool setAsDefault = true);

    /// Saves canvas state to SVG format file as a template file
    void saveTemplate(const QUrl& file);

    /// Check if canvas is saved
    bool isSaved();

    /// Check if canvas is loaded from template (read only)
    bool isTemplate() const;

    /// Set selection mode
    void setSelectionMode(SelectionMode mode);

    PLEScene* scene() const
    {
        return m_scene;
    }

    LayersModel* model() const;

    LayersSelectionModel* selectionModel() const;

    QUndoStack* undoStack() const
    {
        return m_undo_stack;
    }

    PLECanvasSize canvasSize() const;
    void setPLECanvasSize(const PLECanvasSize& size);

    void preparePrinter(QPrinter* printer);

    operator PLEScene*()
    {
        return m_scene;
    }

    operator LayersModel*()
    {
        return this->model();
    }

    operator LayersSelectionModel*()
    {
        return this->selectionModel();
    }

    operator QUndoStack*()
    {
        return m_undo_stack;
    }

public Q_SLOTS:

    void setAntialiasing(bool antialiasing);

    void enable()
    {
        this->setEnabled(true);
    }

    /// Progress state update event
    void progressEvent(ProgressEvent* event);

    void addImage(const QImage& image);
    void addImage(const QUrl& imageUrl);
    void addImages(const QList<QUrl>& images);
    void addText(const QString& text);

    /// Creates move rows command and pushes it onto the stack
    void moveRowsCommand(const QModelIndex& startIndex, int count, const QModelIndex& parentIndex, int move, const QModelIndex& destinationParent);

    /// Move selected items up on scene & model. (Called by layers tree)
    void moveSelectedRowsUp();

    /// Move selected items down on scene & model. (Called by layers tree)
    void moveSelectedRowsDown();

    /// Remove item selected on scene (remove from scene & model => calls removeComand())
    void removeItem(AbstractPhoto* item);

    /// Remove items selected on scene (remove from scene & model => calls removeComand())
    void removeItems(const QList<AbstractPhoto*>& items);

    /// Remove items selected on model (remove from model & scene => calls removeComand())
    void removeSelectedRows();

    /// Select items on model (synchronize model with scene)
    void selectionChanged();

    /// Select items on scene (synchronize scene with model)
    void selectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection);

    /// Conrtols saved-state of the canvas
    void isSavedChanged(int currentCommandIndex);
    void isSavedChanged(bool isStackClean);

    /// Draws whole canvas onto the QPaintDevice
    void renderPLECanvas(QPaintDevice* device);

    /// Draws whole canvas content onto the printer
    void renderPLECanvas(QPrinter* device);

    /// Groups operations into one undo operation
    void beginRowsRemoving();

    /// Finish group of undo operations
    void endRowsRemoving();

    /// Sets selecting mode
    void enableDefaultSelectionMode();

    /// Sets viewing mode
    void enableViewingMode();

    /// Sets zooming mode
    void enableZoomingMode();

    /// Sets canvas editing mode
    void enablePLECanvasEditingMode();

    /// Sets effects editing mode
    void enableEffectsEditingMode();

    /// Sets text editing mode
    void enableTextEditingMode();

    /// Sets rotating mode
    void enableRotateEditingMode();

    /// Sets scaling mode
    void enableScaleEditingMode();

    /// Sets cropping mode
    void enableCropEditingMode();

    /// Sets borders editing mode
    void enableBordersEditingMode();

    /// Refresh widgets connections to canvas signals
    void refreshWidgetConnections(bool isVisible);

    /// Appends new undo command
    void newUndoCommand(QUndoCommand* command);

Q_SIGNALS:

    void hasSelectionChanged(bool hasSelection);
    void selectedItem(AbstractPhoto* photo);
    void setInitialValues(qreal width, Qt::PenJoinStyle cornersStyle, const QColor& color);
    void savedStateChanged();

protected Q_SLOTS:

    /// Used when new item has been created and needs to be added to the scene and to the model
    void addNewItem(AbstractPhoto* item);
    void imageLoaded(const QUrl& url, const QImage& image);

private Q_SLOTS:

    void savingFinished();

private:

    explicit PLECanvas(PLEScene* scene, QWidget* parent = nullptr);

    void init();
    void setupGUI();
    void prepareSignalsConnection();

private:

    QUrl          m_file;
    bool          m_is_saved;
    int           m_saved_on_index;

    PLEScene*        m_scene;
    QUndoStack*   m_undo_stack;
    double        m_scale_factor;

    SelectionMode m_selection_mode;

private:

    PLECanvasPrivate* d;

    friend class PLECanvasPrivate;
    friend class PLECanvasSavingThread;
};

} // namespace PhotoLayoutsEditor

#endif // PLE_CANVAS_H
