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

#ifndef ROTATION_WIDGET_ITEM_P_H
#define ROTATION_WIDGET_ITEM_P_H

// Qt includes

#include <qmath.h>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

// Local includes

#include "abstractphoto.h"

namespace PhotoLayoutsEditor
{

class RotationWidgetItemPrivate;
class RotateItemCommand;

class RotationWidgetItem : public AbstractItemInterface
{
    Q_OBJECT

    RotationWidgetItemPrivate* d;

public:

    explicit RotationWidgetItem(const QList<AbstractPhoto*>& items, QGraphicsItem* parent = nullptr);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    QPainterPath shape() const override;
    QPainterPath opaqueArea() const override;
    QRectF boundingRect() const override;
    void initRotation(const QPainterPath& path, const QPointF& rotationPoint);
    void reset();
    qreal angle() const;
    QPointF rotationPoint() const;
    bool isRotated() const;

protected:

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void setItems(const QList<AbstractPhoto*>& items);

Q_SIGNALS:

    void rotationChanged(const QPointF& point, qreal angle);
    void rotationFinished(const QPointF& point, qreal angle);

    friend class QGraphicsEditingWidget;
    friend class RotateItemCommand;
};

} // namespace PhotoLayoutsEditor

#endif // ROTATION_WIDGET_ITEM_P_H
