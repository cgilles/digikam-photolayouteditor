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
 * Copyright (C) 2020      by Marcel Mathis <maeseee at gmail dot com>
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

#ifndef CROP_WIDGET_ITEM_H
#define CROP_WIDGET_ITEM_H

// Local includes

#include "abstractiteminterface.h"

namespace PhotoLayoutsEditor
{

class AbstractPhoto;
class CropWidgetItemPrivate;

class CropWidgetItem : public AbstractItemInterface
{
    Q_OBJECT

    CropWidgetItemPrivate* d;

public:

    explicit CropWidgetItem(QGraphicsItem* parent = nullptr, QGraphicsScene* scene = nullptr);
    ~CropWidgetItem() override;

    QRectF boundingRect() const override;
    QPainterPath opaqueArea() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

    void setItems(const QList<AbstractPhoto*>& items);

Q_SIGNALS:

    void cropShapeSelected(const QPainterPath& shape);
    void cancelCrop();

private Q_SLOTS:

    void updateShapes();

private:
    /**
     * @brief calculates the normal line starting from the point point through the line line
     * @param line line for intersection (the normal line should show in direction point)
     * @param point starting point of the normal line
     * @return normal line
     */
    QLineF normalLine(const QLineF& line, const QPointF& point) const;

    /**
     * @brief A closed polygon has the same end point as the start point. Use this function to ensure to write both
     * @param value QPointF to write on the corner postion
     * @param polygon polygon to write on
     * @param corner corner to write
     */
    void writeOnPolygon(const QPointF& value, QPolygonF& polygon, uint corner);

    friend class CropWidgetItemPrivate;
};

} // namespace PhotoLayoutsEditor

#endif // CROP_WIDGET_ITEM_H
