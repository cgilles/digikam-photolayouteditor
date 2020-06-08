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

#ifndef CROPWIDGETITEM_H
#define CROPWIDGETITEM_H

#include "abstractiteminterface.h"

namespace PhotoLayoutsEditor
{
    class AbstractPhoto;
    class CropWidgetItemPrivate;

    class CropWidgetItem : public AbstractItemInterface
    {
            Q_OBJECT

            CropWidgetItemPrivate * d;

        public:

            explicit CropWidgetItem(QGraphicsItem * parent = nullptr, QGraphicsScene * scene = nullptr);
            virtual ~CropWidgetItem();

            virtual QRectF boundingRect() const override;
            virtual QPainterPath opaqueArea() const override;
            virtual QPainterPath shape() const override;
            virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

            virtual void keyPressEvent(QKeyEvent * event) override;
            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
            virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) override;

            void setItems(const QList<AbstractPhoto*> & items);

        Q_SIGNALS:

            void cropShapeSelected(const QPainterPath& shape);
            void cancelCrop();

        private Q_SLOTS:

            void updateShapes();

        friend class CropWidgetItemPrivate;
    };
}

#endif // CROPWIDGETITEM_H
