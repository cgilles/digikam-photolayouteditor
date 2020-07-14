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
          pressedVHandler(-1),
          pressedHHandler(-1)
    {
    }

    enum
    {
        Top,
        VCenter,
        Bottom
    };

    enum
    {
        Left,
        HCenter,
        Right
    };

    CropWidgetItemPrivate()
        : m_item(nullptr),
          currentViewTransform(1, 0, 0,    0, 1, 0,   0, 0, 1),
          pressedVHandler(-1),
          pressedHHandler(-1)
    {
        m_handlers[Top][Left] = QRectF(0, 0, 20, 20);
        m_handlers[Top][HCenter] = QRectF(0, 0, 20, 20);
        m_handlers[Top][Right] = QRectF(0, 0, 20, 20);
        m_handlers[VCenter][Left] = QRectF(0, 0, 20, 20);
        m_handlers[VCenter][Right] = QRectF(0, 0, 20, 20);
        m_handlers[Bottom][Left] = QRectF(0, 0, 20, 20);
        m_handlers[Bottom][HCenter] = QRectF(0, 0, 20, 20);
        m_handlers[Bottom][Right] = QRectF(0, 0, 20, 20);
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
    QRectF m_begin_rect;
    QRectF m_handlers[Bottom+1][Right+1];
    QPainterPath m_elipse;
    int pressedVHandler;
    int pressedHHandler;
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

    for ( uint vertical = 0; vertical < 3; ++vertical)
    {
        for ( uint horizontal = 0; horizontal < 3; ++horizontal)
        {
            if ( vertical != VCenter || horizontal != HCenter)
            {
                m_handlers[horizontal][vertical].setWidth(tw);
                m_handlers[horizontal][vertical].setHeight(th);
            }

        }
    }

    m_elipse = QPainterPath();
    m_elipse.addEllipse(rect.center(), tw / 2, th / 2);

    w = qAbs(rect.width()) + 7 * tempx;
    w = (w < 0 ? w / 2.0 : 0);
    h = qAbs(rect.height()) + 7 * tempy;
    h = (h < 0 ? h / 2.0 : 0);
    m_handlers[Top][Left].moveCenter(rect.topLeft() + QPointF(w,h));
    m_handlers[Top][HCenter].moveCenter( QPointF( rect.center().x(), rect.top() + h ) );
    m_handlers[Top][Right].moveCenter(rect.topRight() + QPointF(-w,h));
    m_handlers[VCenter][Left].moveCenter( QPointF( rect.left() + w, rect.center().y() ) );
    m_handlers[VCenter][Right].moveCenter( QPointF( rect.right() - w, rect.center().y() ) );
    m_handlers[Bottom][Left].moveCenter(rect.bottomLeft() + QPointF(w,-h));
    m_handlers[Bottom][HCenter].moveCenter( QPointF( rect.center().x(), rect.bottom() - h ) );
    m_handlers[Bottom][Right].moveCenter(rect.bottomRight() + QPointF(-w,-h));

    m_shape = QPainterPath();
    m_shape.addRect(rect);

    m_handlers_path = QPainterPath();

    for (int i = Top; i <= Bottom; ++i)
        for (int j = Left; j <= Right; ++j)
            m_handlers_path.addRect(m_handlers[i][j]);

    m_handlers_path += m_elipse;

    m_item_shape = QPainterPath();
    m_item_shape.setFillRule(Qt::WindingFill);

    if (m_item->scene())
        m_item_shape.addRect( m_item->mapRectFromScene(m_item->scene()->sceneRect()) );

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
    p1.addRect(d->m_polygon.boundingRect());
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
        QRectF rect = d->m_polygon.boundingRect();
        if (rect.height() > 1 && rect.width() > 1)
        {
            QPainterPath p;
            p.addRect( rect );

            bool commandGroupOpened = false;

            if (d->m_items.count() > 1)
            {
                commandGroupOpened = true;
                PLEWindow::instance()->beginUndoCommandGroup(QObject::tr("Crop items"));
            }

            foreach (AbstractPhoto* const item, d->m_items)
                item->setCropShape( this->mapToItem(item, p) );

            if (commandGroupOpened)
                PLEWindow::instance()->endUndoCommandGroup();
        }
        else
        {
            QMessageBox::critical(nullptr,
                                  QObject::tr("Error"),
                                  QObject::tr("Bounding rectangle of the crop shape has size [%1px x %2px] "
                                              "and it's less than 1px x 1px")
                                              .arg(QString::number(qRound(rect.width())))
                                              .arg(QString::number(qRound(rect.height()))));
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
    d->pressedVHandler = -1;
    d->pressedHHandler = -1;
    d->handlerOffset = QPointF(0,0);
    d->m_begin_rect = d->m_polygon.boundingRect();
    this->setFocus( Qt::MouseFocusReason );

    if (event->button() == Qt::LeftButton)
    {
        QPointF handledPoint = this->mapFromScene(event->buttonDownScenePos(Qt::LeftButton));

        for (int i = CropWidgetItemPrivate::Top; i <= CropWidgetItemPrivate::Bottom; ++i)
        {
            for (int j = CropWidgetItemPrivate::Left; j <= CropWidgetItemPrivate::Right; ++j)
            {
                if (d->m_handlers[i][j].contains(handledPoint))
                {
                    d->pressedVHandler = i;
                    d->pressedHHandler = j;
                    goto found;
                }
            }
        }

        if (d->m_elipse.contains(handledPoint))
        {
            d->pressedVHandler = CropWidgetItemPrivate::VCenter;
            d->pressedHHandler = CropWidgetItemPrivate::HCenter;
            event->setAccepted(true);
        }
        else if (d->m_shape.contains(handledPoint))
        {
            event->setAccepted(true);
        }

        return;

        found:

            d->handlerOffset = d->m_handlers[d->pressedVHandler][d->pressedHHandler].center() - handledPoint;
            event->setAccepted(true);
    }
}

void CropWidgetItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (d->pressedHHandler == -1 || d->pressedVHandler == -1)
        return;

    QRectF maxRect = d->m_crop_shape.boundingRect();

    // New handler position calc

    QPointF point = event->pos() + d->handlerOffset;

    if      (point.rx() < maxRect.left())
        point.setX( maxRect.left() );
    else if (point.rx() > maxRect.right())
        point.setX( maxRect.right() );

    if      (point.ry() < maxRect.top())
        point.setY( maxRect.top() );
    else if (point.ry() > maxRect.bottom())
        point.setY( maxRect.bottom() );

    QRectF temp = d->m_polygon.boundingRect();

    // Position change

    if (d->pressedVHandler == CropWidgetItemPrivate::VCenter &&
        d->pressedHHandler == CropWidgetItemPrivate::HCenter)
    {
        QPointF dif = event->scenePos() - event->lastScenePos();
        temp.translate(dif);

        temp.translate( qMin<float>(maxRect.right()-temp.right(),0.0), qMin<float>(maxRect.bottom()-temp.bottom(),0.0) );
        temp.translate( qMax<float>(maxRect.left()-temp.left(),0.0), qMax<float>(maxRect.top()-temp.top(),0.0) );
    }
    else
    {
        // Size change

        // Vertical size change

        if      (d->pressedVHandler == CropWidgetItemPrivate::Top)
            temp.setTop( point.y() );
        else if (d->pressedVHandler == CropWidgetItemPrivate::Bottom)
            temp.setBottom( point.y() );

        // Horizontal size change

        if      (d->pressedHHandler == CropWidgetItemPrivate::Right)
            temp.setRight( point.x() );
        else if (d->pressedHHandler == CropWidgetItemPrivate::Left)
            temp.setLeft( point.x() );

        // Keeps aspect ratio

        if (event->modifiers() & Qt::ShiftModifier)
        {
            qreal xFactor = temp.width()  / d->m_begin_rect.width();
            qreal yFactor = temp.height() / d->m_begin_rect.height();

            if      (d->pressedHHandler == CropWidgetItemPrivate::HCenter)
            {
                qreal dif = d->m_begin_rect.width() - d->m_begin_rect.width() * yFactor;
                temp.setRight( d->m_begin_rect.right() - dif / 2.0 );
                temp.setLeft( d->m_begin_rect.left() + dif / 2.0 );
            }
            else if (d->pressedVHandler == CropWidgetItemPrivate::VCenter)
            {
                qreal dif = d->m_begin_rect.height() - d->m_begin_rect.height() * xFactor;
                temp.setTop( d->m_begin_rect.top() + dif / 2.0 );
                temp.setBottom( d->m_begin_rect.bottom() - dif / 2.0 );
            }
            else if (xFactor > yFactor)
            {
                qreal dif = d->m_begin_rect.width() - d->m_begin_rect.width() * yFactor;
                if (d->pressedHHandler == CropWidgetItemPrivate::Right)
                    temp.setRight( d->m_begin_rect.right() - dif );
                else if (d->pressedHHandler == CropWidgetItemPrivate::Left)
                    temp.setLeft( d->m_begin_rect.left() + dif );
            }
            else if (xFactor < yFactor)
            {
                qreal dif = d->m_begin_rect.height() - d->m_begin_rect.height() * xFactor;
                if (d->pressedVHandler == CropWidgetItemPrivate::Top)
                    temp.setTop( d->m_begin_rect.top() + dif );
                else if (d->pressedVHandler == CropWidgetItemPrivate::Bottom)
                    temp.setBottom( d->m_begin_rect.bottom() - dif );
            }
        }

        temp.setBottom( qMin(temp.bottom() , maxRect.bottom()) );
        temp.setTop( qMax(temp.top() , maxRect.top()) );
        temp.setLeft( qMax(temp.left() , maxRect.left()) );
        temp.setRight( qMin(temp.right() , maxRect.right()) );

        // Rect inverse

        if (temp.height() < 0)
        {
            qreal t = temp.bottom();
            temp.setBottom(temp.top());
            temp.setTop(t);
            d->pressedVHandler = (d->pressedVHandler == CropWidgetItemPrivate::Top ? CropWidgetItemPrivate::Bottom :CropWidgetItemPrivate::Top);
        }

        if (temp.width() < 0)
        {
            qreal t = temp.right();
            temp.setRight(temp.left());
            temp.setLeft(t);
            d->pressedHHandler = (d->pressedHHandler == CropWidgetItemPrivate::Left ? CropWidgetItemPrivate::Right :CropWidgetItemPrivate::Left);
        }

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

} // namespace PhotoLayoutsEditor
