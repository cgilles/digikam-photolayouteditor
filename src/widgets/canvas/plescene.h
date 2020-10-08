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

#ifndef PLE_SCENE_H
#define PLE_SCENE_H

// Qt includes

#include <QGraphicsScene>
#include <QPainter>
#include <QColor>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsItem>

// Local includes

#include "photoitem.h"
#include "textitem.h"

namespace PhotoLayoutsEditor
{

class PLEScenePrivate;

// My classes declarations
class PLECanvas;
class PLESceneBackground;
class PLESceneBorder;
class MousePressListener;
class LayersModel;
class LayersSelectionModel;

// Undo commands
class AddItemsCommand;
class MoveItemsCommand;
class RemoveItemsCommand;
class RotateItemCommand;
class CropItemsCommand;

class ProgressObserver;

class PLEScene : public QGraphicsScene
{
    Q_OBJECT

    PLEScenePrivate* d;

    enum EditMode
    {
        View = 0,
        Selecting = 1,
        Moving = 2,
        MouseTracking = 4,
        OneclickFocusItems = 8,
        Rotating = 16,
        Scaling = 32,
        Cropping = 64
    };

    enum SelectionMode
    {
        NoSelection = 0,
        SingleSelection = 1,
        MultiSelection = 2
    };

public:

    explicit PLEScene(const QRectF& dimension, QObject* const parent = nullptr);
    ~PLEScene() override;

    PLESceneBackground* background();
    PLESceneBorder* border();

    LayersModel* model() const;
    LayersSelectionModel* selectionModel() const;

    void addItem(AbstractPhoto* photo);
    void addItems(const QList<AbstractPhoto*>& item);
    void removeItem(AbstractPhoto* item);
    void removeItems(const QList<AbstractPhoto*>& items);
    QList<AbstractPhoto*> selectedItems() const;
    void setInteractionMode(int mode);
    void setSelectionMode(SelectionMode selectionMode);
    bool isSelectionVisible();
    QDomDocument toSvg(ProgressObserver* observer);
    QDomDocument toTemplateSvg(ProgressObserver* observer);
    QDomDocument toSvg(ProgressObserver* observer, bool asTemplate);
    static PLEScene* fromSvg(QDomElement& svgImage);
    void addSelectingFilter(const QMetaObject & classMeta);
    void clearSelectingFilters();
    void setRotationWidgetVisible(bool isVisible);
    void setScalingWidgetVisible(bool isVisible);
    void setCropWidgetVisible(bool isVisible);
    qreal gridHorizontalDistance() const;
    qreal gridVerticalDistance() const;

    const QGraphicsScene* toGraphicsPLEScene() const
    {
        return this;
    }

    QGraphicsScene* toGraphicsPLEScene()
    {
        return this;
    }

    void render(QPainter* painter, const QRectF& target = QRectF(), const QRectF& source = QRectF(), Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio);

    void readPLESceneMousePress(MousePressListener * mouseListener);

Q_SIGNALS:

    void finishEditing(const QPainterPath& path);
    void itemAboutToBeRemoved(AbstractPhoto* item);
    void itemsAboutToBeRemoved(const QList<AbstractPhoto*>& items);
    void mousePressedPoint(const QPointF& point);

public Q_SLOTS:

    void removeSelectedItems();
    void changeSelectedImage();
    void setGrid(double x, double y);
    void setHorizontalGrid(double x);
    void setVerticalGrid(double y);
    void setGridVisible(bool visible);
    bool isGridVisible();
    void updateSelection();
    void setSelectionVisible(bool isVisible);
    void closeCropWidget();

protected:

    virtual void contextMenuEvent(QGraphicsSceneMouseEvent* event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;
    void drawForeground(QPainter* painter, const QRectF& rect) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private Q_SLOTS:

    void imageLoaded(const QUrl& url, const QImage& image);
    void calcSelectionBoundingRect();

private:

    PLEScene(const PLEScene&);
    PLEScene& operator=(const PLEScene&);

    bool askAboutRemoving(int count);
    bool canDecode(const QMimeData* mimeData);

private:

    int                        m_interaction_mode;

    SelectionMode              selectionMode;

    QGraphicsPathItem*         temp_widget;
    QPainterPath               temp_path;

    // Grid properties
    qreal                      x_grid;
    qreal                      y_grid;
    bool                       grid_visible;
    QGraphicsItemGroup*        grid_item;
    bool                       grid_changed;

    static const SelectionMode DEFAULT_SELECTING_MODE = MultiSelection;
    static const int           DEFAULT_EDITING_MODE   = Moving & Selecting;
    static QColor              OUTSIDE_SCENE_COLOR;

    friend class PLECanvas;
    friend class PLECanvasWidget;
    friend class PLEScenePrivate;
    friend class AbstractPhoto;
    friend class MoveItemsCommand;
};

} // namespace PhotoLayoutsEditor

#endif // PLE_SCENE_H
