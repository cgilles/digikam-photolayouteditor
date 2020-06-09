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

#ifndef ROTATIONWIDGETITEM_P_H
#define ROTATIONWIDGETITEM_P_H

#include <qmath.h>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "abstractphoto.h"

namespace PhotoLayoutsEditor
{
    class RotationWidgetItemPrivate;
    class RotateItemCommand;

    class RotationWidgetItem : public AbstractItemInterface
    {
            Q_OBJECT

            RotationWidgetItemPrivate * d;

        public:

            explicit RotationWidgetItem(const QList<AbstractPhoto*> & items, QGraphicsItem* parent = nullptr);
            virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
            virtual QPainterPath shape() const override;
            virtual QPainterPath opaqueArea() const override;
            virtual QRectF boundingRect() const override;
            void initRotation(const QPainterPath& path, const QPointF& rotationPoint);
            void reset();
            qreal angle() const;
            QPointF rotationPoint() const;
            bool isRotated() const;

        protected:

            virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
            virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;
            virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

            void setItems(const QList<AbstractPhoto*> & items);

        Q_SIGNALS:

            void rotationChanged(const QPointF& point, qreal angle);
            void rotationFinished(const QPointF& point, qreal angle);

        friend class QGraphicsEditingWidget;
        friend class RotateItemCommand;
    };

}

#endif // ROTATIONWIDGETITEM_P_H
