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

#ifndef SCALING_WIDGET_ITEM_H
#define SCALING_WIDGET_ITEM_H

// Local includes

#include "abstractiteminterface.h"

namespace PhotoLayoutsEditor
{

class AbstractPhoto;
class ScalingWidgetItemPrivate;

class MoveItemCommand;
class ScaleItemCommand;

class ScalingWidgetItem : public AbstractItemInterface
{
    Q_OBJECT

    ScalingWidgetItemPrivate* d;

public:

    explicit ScalingWidgetItem(const QList<AbstractPhoto*>& items, QGraphicsItem* parent = nullptr, QGraphicsScene* scene = nullptr);
    ~ScalingWidgetItem() override;

    QRectF boundingRect() const override;
    QPainterPath opaqueArea() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:

    void setScaleItems(const QList<AbstractPhoto*>& items);

private Q_SLOTS:

    void updateShapes();

    friend class ScalingWidgetItemPrivate;

    friend class MoveItemCommand;
    friend class ScaleItemCommand;
};

} // namespace PhotoLayoutsEditor

#endif // SCALING_WIDGET_ITEM_H
