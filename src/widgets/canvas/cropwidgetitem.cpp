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

#include "cropwidgetitem.h"

// C++ includes

#include <limits>

// Qt includes

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>

// Local includes

#include "abstractphoto.h"
#include "plewindow.h"

namespace PhotoLayoutsEditor
{

class CropWidgetItemPrivate
{
    explicit CropWidgetItemPrivate (CropWidgetItem* item)
        : m_item(item),
          pressedHandler(-1)
    {
    }

    enum
    {
        TopLeft,
        TopRight,
        BottomRight,
        BottomLeft,
        Center
    };

    CropWidgetItemPrivate()
        : m_item(nullptr),
          currentViewTransform(1, 0, 0,    0, 1, 0,   0, 0, 1),
          pressedHandler(-1)
    {
        m_handlers[TopLeft]     = QRectF(0, 0, 20, 20);
        m_handlers[TopRight]    = QRectF(0, 0, 20, 20);
        m_handlers[BottomRight] = QRectF(0, 0, 20, 20);
        m_handlers[BottomLeft]  = QRectF(0, 0, 20, 20);
        m_elipse.addEllipse(QPointF(0,0), 10, 10);
    }

    CropWidgetItem* m_item;

    QTransform currentViewTransform;
    void transformDrawings(const QTransform& viewTransform);
    void calculateDrawings();

    QList<AbstractPhoto*> m_items;
    QPainterPath m_crop_shape;
    QPainterPath m_shape;
    QPainterPath m_handlers_path;
    QPainterPath m_item_shape;
    QPolygonF m_polygon;
    QRectF m_handlers[BottomLeft+1];
    QPainterPath m_elipse;
    int pressedHandler;
    QPointF handlerOffset;

    friend class CropWidgetItem;
};

void CropWidgetItemPrivate::transformDrawings(const QTransform& viewTransform)
{
    if (currentViewTransform == viewTransform)
        return;

    currentViewTransform = viewTransform;

    this->calculateDrawings();
}

void CropWidgetItemPrivate::calculateDrawings()
{
    // Scale handlers
    qreal tempx = -10 / currentViewTransform.m11();
    qreal tempy = -10 / currentViewTransform.m22();

    QRectF rect = m_polygon.boundingRect();
    qreal w = qAbs(rect.width()) + 12 * tempx;
    w = (w < 0 ? w / 3.0 : 0);
    w = (w < tempx ? tempx : w);
    qreal tw = w - 4 * tempx;

    qreal h = qAbs(rect.height()) + 12 * tempy;
    h = (h < 0 ? h / 3.0 : 0);
    h = (h < tempy ? tempy : h);
    qreal th = h - 4 * tempy;

    for (int i = TopLeft; i <= BottomLeft; ++i)
    {
        m_handlers[i].setWidth(tw);
        m_handlers[i].setHeight(th);
    }

    m_elipse = QPainterPath();
    m_elipse.addEllipse(rect.center(), tw / 2, th / 2);

    m_handlers[TopLeft].moveCenter(m_polygon[TopLeft]);
    m_handlers[TopRight].moveCenter(m_polygon[TopRight]);
    m_handlers[BottomLeft].moveCenter(m_polygon[BottomLeft]);
    m_handlers[BottomRight].moveCenter(m_polygon[BottomRight]);

    m_shape = QPainterPath();
    m_shape.addRect(rect);

    m_handlers_path = QPainterPath();

    for (int i = TopLeft; i <= BottomLeft; ++i)
        m_handlers_path.addRect(m_handlers[i]);

    m_handlers_path += m_elipse;

    m_item_shape = QPainterPath();
    m_item_shape.setFillRule(Qt::WindingFill);

    if (m_item->scene())
        m_item_shape.addRect(m_item->mapRectFromScene(m_item->scene()->sceneRect()));

    m_item_shape = m_item_shape.united(m_handlers_path);
}

CropWidgetItem::CropWidgetItem(QGraphicsItem* parent, QGraphicsScene* scene)
    : AbstractItemInterface(parent, scene),
      d(new CropWidgetItemPrivate(this))
{
    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, false);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setZValue(std::numeric_limits<double>::infinity());
}

CropWidgetItem::~CropWidgetItem()
{
    delete d;
}

QRectF CropWidgetItem::boundingRect() const
{
    return d->m_item_shape.boundingRect();
}

QPainterPath CropWidgetItem::opaqueArea() const
{
    return d->m_item_shape;
}

QPainterPath CropWidgetItem::shape() const
{
    return d->m_item_shape;
}

void CropWidgetItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* widget)
{
    // Get the view
    QGraphicsView* view = qobject_cast<QGraphicsView*>(widget->parentWidget());

    if (!view)
        return;

    QTransform viewTransform = view->transform();
    d->transformDrawings(viewTransform);

    painter->save();

    QPainterPath p;
    p.setFillRule(Qt::WindingFill);
    p.addPolygon(this->mapFromScene(this->scene()->sceneRect()));
    p.addPath(d->m_crop_shape);
    QPainterPath p1;
    p1.addPolygon(d->m_polygon);
    p -= p1;
    painter->fillPath(p, QColor(0, 0, 0, 150));

    // Draw bounding rect
    painter->setCompositionMode(QPainter::RasterOp_NotSourceAndNotDestination);
    painter->setPen(Qt::black);
    painter->setPen(Qt::DashLine);
    painter->drawPath(d->m_crop_shape);
    painter->setPen(Qt::red);
    painter->setPen(Qt::SolidLine);
    painter->drawPath(d->m_shape);
    painter->drawPath(d->m_handlers_path);
    painter->drawPath(d->m_elipse);

    painter->restore();
}

void CropWidgetItem::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return)
    {
        if (not d->m_polygon.isEmpty())
        {
            QPainterPath p;
            p.addPolygon(d->m_polygon);

            bool commandGroupOpened = false;

            if (d->m_items.count() > 1)
            {
                commandGroupOpened = true;
                PLEWindow::instance()->beginUndoCommandGroup(QObject::tr("Crop items"));
            }

            foreach (AbstractPhoto* const item, d->m_items)
                item->setCropShape(this->mapToItem(item, p));

            if (commandGroupOpened)
                PLEWindow::instance()->endUndoCommandGroup();
        }
        else
        {
            QMessageBox::critical(nullptr,
                                  QObject::tr("Error"),
                                  QObject::tr("Bounding polygon is empty"));
        }

        event->setAccepted(true);
    }
    else if (event->key() == Qt::Key_Escape)
    {
        Q_EMIT cancelCrop();
        event->setAccepted(true);
    }
}

void CropWidgetItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    event->setAccepted(false);
    d->pressedHandler = -1;
    d->handlerOffset = QPointF(0,0);
    this->setFocus( Qt::MouseFocusReason );

    if (event->button() == Qt::LeftButton)
    {
        QPointF handledPoint = this->mapFromScene(event->buttonDownScenePos(Qt::LeftButton));

        for (int i = CropWidgetItemPrivate::TopLeft; i <= CropWidgetItemPrivate::BottomLeft; ++i)
        {
            if (d->m_handlers[i].contains(handledPoint))
            {
                d->pressedHandler = i;
                goto found;
            }
        }

        if (d->m_elipse.contains(handledPoint))
        {
            d->pressedHandler = CropWidgetItemPrivate::Center;
            event->setAccepted(true);
        }
        else if (d->m_shape.contains(handledPoint))
        {
            event->setAccepted(true);
        }

        return;

        found:

            d->handlerOffset = d->m_handlers[d->pressedHandler].center() - handledPoint;
            event->setAccepted(true);
    }
}

void CropWidgetItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (d->pressedHandler == -1)
        return;

    QPolygonF maxPolygon = d->m_crop_shape.toFillPolygon();

    // New handler position calc

    QPointF point = event->pos() + d->handlerOffset;

    QPolygonF temp = d->m_polygon;

    // Size change

    uint corner = 0;
    if      (d->pressedHandler == CropWidgetItemPrivate::TopLeft)
                corner = CropWidgetItemPrivate::TopLeft;
    else if (d->pressedHandler == CropWidgetItemPrivate::TopRight)
                 corner = CropWidgetItemPrivate::TopRight;
    else if (d->pressedHandler == CropWidgetItemPrivate::BottomLeft)
                corner = CropWidgetItemPrivate::BottomLeft;
    else if (d->pressedHandler == CropWidgetItemPrivate::BottomRight)
                 corner = CropWidgetItemPrivate::BottomRight;
    else
        QMessageBox::critical(nullptr,
                              QObject::tr("Error"),
                              QObject::tr("Unsupportet event"));

    // Define corners

    uint afterCorner = (corner + 1) % 4;
    uint beforeCorner = (corner + 3) % 4;
    uint oppositCorner = (corner + 2) % 4;

    // Set points

    if (maxPolygon.containsPoint(point, Qt::WindingFill))
        writeOnPolygon(point, temp, corner);
    else
    {
        // Before
        QLineF beforeLine = QLineF(temp[corner], temp[beforeCorner]);
        QLineF beforeNormalLine = normalLine(beforeLine, point);

        QPointF beforeIntersectionPoint;
        beforeLine.intersect(beforeNormalLine, &beforeIntersectionPoint);

        QLineF beforeMaxLine = QLineF(maxPolygon[corner], maxPolygon[beforeCorner]);
        auto beforeMaxIntersection = beforeMaxLine.intersect(beforeNormalLine, &beforeIntersectionPoint);

        // After
        QLineF afterLine = QLineF(temp[afterCorner], temp[corner]);
        QLineF afterNormalLine = normalLine(afterLine, point);

        QPointF afterIntersectionPoint;
        afterLine.intersect(afterNormalLine, &afterIntersectionPoint);

        QLineF afterMaxLine = QLineF(maxPolygon[corner], maxPolygon[afterCorner]);
        auto afterMaxIntersection = afterMaxLine.intersect(afterNormalLine, &afterIntersectionPoint);

        if (beforeMaxIntersection == QLineF::BoundedIntersection)
            writeOnPolygon(beforeIntersectionPoint, temp, corner);
        else if (afterMaxIntersection == QLineF::BoundedIntersection)
            writeOnPolygon(afterIntersectionPoint, temp, corner);
        else
            writeOnPolygon(maxPolygon[corner], temp, corner);
    }

    // Keeps normal angle ratio

    if (event->modifiers() & Qt::ShiftModifier)
    {
        // Before
        QLineF beforeLine = QLineF(temp[oppositCorner], temp[beforeCorner]);
        QLineF beforeNormalLine = normalLine(beforeLine, point);

        QPointF beforeIntersectionPoint;
        beforeLine.intersect(beforeNormalLine, &beforeIntersectionPoint);

        QLineF beforeMaxLine = QLineF(maxPolygon[beforeCorner], maxPolygon[oppositCorner]);
        QPointF beforeIntersectionMaxPoint;
        auto beforeIntersectionMax = beforeMaxLine.intersect(beforeNormalLine, &beforeIntersectionMaxPoint);

        if(beforeIntersectionMax == QLineF::BoundedIntersection)
            writeOnPolygon(beforeIntersectionPoint, temp, beforeCorner);

        // After
        QLineF afterLine = QLineF(temp[afterCorner], temp[oppositCorner]);
        QLineF afterNormalLine = normalLine(afterLine, point);

        QPointF afterIntersectionPoint;
        afterLine.intersect(afterNormalLine, &afterIntersectionPoint);

        QLineF afterMaxLine = QLineF(maxPolygon[afterCorner], maxPolygon[oppositCorner]);
        QPointF afterIntersectionMaxPoint;
        auto afterIntersectionMax = afterMaxLine.intersect(afterNormalLine, &afterIntersectionMaxPoint);

        if(afterIntersectionMax == QLineF::BoundedIntersection)
            writeOnPolygon(afterIntersectionPoint, temp, afterCorner);
    }

    QPainterPath updatePath;
    updatePath.setFillRule(Qt::WindingFill);
    updatePath.addPolygon(d->m_polygon);
    updatePath = updatePath.united(d->m_handlers_path);

    d->m_polygon = temp;

    updatePath.addPolygon(d->m_polygon);

    event->setAccepted(true);
    d->calculateDrawings();
    updatePath = updatePath.united(d->m_handlers_path);

    this->update(updatePath.boundingRect());
}

void CropWidgetItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* /*event*/)
{
}

void CropWidgetItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* /*event*/)
{
}

void CropWidgetItem::setItems(const QList<AbstractPhoto*>& items)
{
    d->m_items = items;

    foreach (AbstractPhoto* const item, items)
        connect(item, SIGNAL(changed()), this, SLOT(updateShapes()));

    this->updateShapes();
}

void CropWidgetItem::updateShapes()
{
    d->m_crop_shape = QPainterPath();

    foreach (AbstractPhoto* const item, d->m_items)
        d->m_crop_shape += this->mapFromItem(item, item->itemDrawArea());

    QPainterPath temp;

    foreach (AbstractPhoto* const item, d->m_items)
        temp += this->mapFromItem(item, item->itemOpaqueArea());

    d->m_polygon = temp.toFillPolygon();
    d->calculateDrawings();

    this->update();
}

QLineF CropWidgetItem::normalLine(const QLineF& line, const QPointF& point) const
{
    QLineF normalLine = line.normalVector();
    normalLine.translate(point - line.p1());
    qreal inf = 99999999.0; // Do not use qInf() as it may result in NaN on one axis
    normalLine.setLength(inf);

    return normalLine;
}

void CropWidgetItem::writeOnPolygon(const QPointF& value, QPolygonF& polygon, uint corner)
{
    corner = corner % 4;
    polygon[corner] = value;

    if (corner == CropWidgetItemPrivate::TopLeft)
        polygon[CropWidgetItemPrivate::BottomLeft + 1] = value; // the first and last item are the same on a closed object
}

} // namespace PhotoLayoutsEditor
