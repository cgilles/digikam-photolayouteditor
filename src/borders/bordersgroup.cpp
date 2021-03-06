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

#include "bordersgroup.h"

// Qt includes

#include <QGraphicsScene>

// Local includes

#include "borderdrawerinterface.h"
#include "abstractphoto.h"
#include "abstractphoto_p.h"
#include "borderdrawersloader.h"

namespace PhotoLayoutsEditor
{

class BordersGroupPrivate
{
    explicit BordersGroupPrivate(BordersGroup* group)
        : group(group),
          photo(nullptr)
    {
    }

    BordersGroup*                 group;
    AbstractPhoto*                photo;
    QList<BorderDrawerInterface*> borders;
    QPainterPath                  shape;

    friend class BordersGroup;
};

BordersGroup::BordersGroup(AbstractPhoto* const photo)
    : AbstractMovableModel(photo),
      d(new BordersGroupPrivate(this))
{
    d->photo = photo;

    connect(this, SIGNAL(drawersChanged()),
            photo, SLOT(refresh()));
}

BordersGroup::~BordersGroup()
{
    qDebug() << "PhotoEffectsGroup delete";
    delete d;
}

QPainterPath BordersGroup::shape()
{
    if (!graphicsItem())
        return QPainterPath();

    if (d->shape.isEmpty() && d->borders.count())
        calculateShape();

    return d->shape;
}

void BordersGroup::calculateShape()
{
    QPainterPath photoShape = graphicsItem()->itemOpaqueArea();//graphicsItem()->transform().map(graphicsItem()->itemOpaqueArea());
    d->shape = QPainterPath();

    foreach (BorderDrawerInterface* const drawer, d->borders)
    {
        if (drawer)
            d->shape = d->shape.united( drawer->path(photoShape) );
    }

    //d->shape = graphicsItem()->transform().inverted().map(d->shape);
}

AbstractPhoto* BordersGroup::graphicsItem() const
{
    return d->photo;
}

void BordersGroup::paint(QPainter* painter, const QStyleOptionGraphicsItem* option)
{
    painter->save();
    //painter->setTransform( graphicsItem()->transform().inverted() * painter->transform() );

    if (!graphicsItem())
        return;

    for (int i = d->borders.count()-1 ; i >= 0 ; --i)
    {
        BorderDrawerInterface* border = d->borders.at(i);

        if (border)
            border->paint(painter, option);
    }

    painter->restore();
}

void BordersGroup::refresh()
{
    if (!graphicsItem())
        return;

    QRectF updateRect = graphicsItem()->boundingRect();

    this->calculateShape();
    updateRect        = updateRect.united(graphicsItem()->boundingRect());

    if (graphicsItem()->scene())
        graphicsItem()->scene()->update(graphicsItem()->mapRectToScene(updateRect));
    else
        graphicsItem()->update(updateRect);

    Q_EMIT dataChanged(QModelIndex(), QModelIndex());
}

bool BordersGroup::prependDrawer(BorderDrawerInterface* drawer)
{
    return insertDrawer(drawer, 0);
}

bool BordersGroup::insertDrawer(BorderDrawerInterface* drawer, int position)
{
    if (!drawer || (position < 0) || (position > rowCount()) || !insertRow(position))
        return false;

    d->borders.takeAt(position);
    d->borders.insert(position, drawer);

    connect(drawer, SIGNAL(changed()),
            this, SLOT(emitBordersChanged()));

    return true;
}

bool BordersGroup::appendDrawer(BorderDrawerInterface* drawer)
{
    return insertDrawer(drawer, this->rowCount());
}

BorderDrawerInterface* BordersGroup::removeDrawer(int position)
{
    if ((position < 0) || (position >= rowCount()))
        return nullptr;

    BorderDrawerInterface* result = d->borders.at(position);
    removeRow(position);

    return result;
}

bool BordersGroup::moveDrawer(int sourcePosition, int destinationPosition)
{
    return moveRowsData(sourcePosition, 1, destinationPosition);
}

QDomElement BordersGroup::toSvg(QDomDocument& document)
{
    QDomElement result = document.createElement(QLatin1String("g"));
    result.setAttribute(QLatin1String("class"), QLatin1String("borders"));

    for (int i = d->borders.count()-1 ; i >= 0 ; --i)
    {
        BorderDrawerInterface* const drawer = d->borders[i];
        QDomElement temp                    = BorderDrawersLoader::drawerToSvg(drawer, document);

        if (temp.isNull())
            continue;

        result.appendChild(temp);
    }

    return result;
}

BordersGroup* BordersGroup::fromSvg(QDomElement& element, AbstractPhoto* graphicsItem)
{
    // Find borders specific element
    QDomElement bordersElement;
    QDomNodeList children = element.childNodes();

    for (int i = children.count()-1 ; i >= 0 ; --i)
    {
        if (!children.at(i).isElement())
            continue;

        bordersElement = children.at(i).toElement();

        if ((bordersElement.tagName() != QLatin1String("g")) ||
            (bordersElement.attribute(QLatin1String("class")) != QLatin1String("borders")))
        {
            bordersElement = QDomElement();
            continue;
        }

        break;
    }

    // If not found return 0 == Corrupted or invalid file

    if (bordersElement.isNull())
        return nullptr;

    // Load drawers in loop
    BordersGroup* result = new BordersGroup(nullptr);
    children             = bordersElement.childNodes();

    for (int i = children.count()-1 ; i >= 0 ; --i)
    {
        QDomNode child = children.at(i);
        QDomElement childElement;

        if (!child.isElement() || (childElement = child.toElement()).isNull())
            continue;

        // Create drawer from DOM element

        BorderDrawerInterface* const drawer = BorderDrawersLoader::getDrawerFromSvg(childElement);

        if (!drawer)
            continue;

        // Insert it into model

        result->d->borders.push_back(drawer);
        drawer->setGroup(result);
        drawer->setParent(result);
    }

    result->d->photo = graphicsItem;

    return result;
}

QObject* BordersGroup::item(const QModelIndex& index) const
{
    if (index.isValid() && (index.row() < d->borders.count()))
        return d->borders.at(index.row());

    return nullptr;
}

void BordersGroup::setItem(QObject* item, const QModelIndex& index)
{
    BorderDrawerInterface* const drawer = dynamic_cast<BorderDrawerInterface*>(item);

    if (!drawer || !index.isValid())
        return;

    int row = index.row();

    if ((row < 0) || (row >= rowCount()))
        return;

    if (drawer == d->borders.at(row))
        return;

    BorderDrawerInterface* const temp = d->borders.takeAt(row);

    if (temp)
        temp->disconnect(this);

    d->borders.insert(row, drawer);

    connect(drawer, SIGNAL(changed()),
            this, SLOT(emitBordersChanged()));

    drawer->setGroup(this);
    this->refresh();
}

int BordersGroup::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return 1;
}

QVariant BordersGroup::data(const QModelIndex& index, int role) const
{
    if ((role == Qt::DisplayRole) && hasIndex(index.row(), index.column(), index.parent()))
    {
        BorderDrawerInterface* const border = d->borders.at(index.row());

        if (border)
            return border->toString();
    }

    return QVariant();
}

QModelIndex BordersGroup::index(int row, int column, const QModelIndex& parent) const
{
    if ((row >= rowCount(parent)) || (column >= columnCount(parent)))
        return QModelIndex();

    return createIndex(row, column, d->borders.at(row));
}

bool BordersGroup::insertRows(int row, int count, const QModelIndex& parent)
{
    if ((row > rowCount(parent)) || (count < 0))
        return false;

    beginInsertRows(parent, row, row+count-1);

    while (count--)
        d->borders.insert(row, nullptr);

    endInsertRows();

    Q_EMIT layoutChanged();

    return true;
}

QModelIndex BordersGroup::parent(const QModelIndex& /*child*/) const
{
    return QModelIndex();
}

bool BordersGroup::removeRows(int row, int count, const QModelIndex& parent)
{
    if ((row >= rowCount(parent)) || (count <= 0) || (row+count > rowCount(parent)))
        return false;

    beginRemoveRows(QModelIndex(), row, row+count-1);

    while (count--)
        d->borders.removeAt(row);

    endRemoveRows();

    this->refresh();

    Q_EMIT layoutChanged();

    return true;
}

int BordersGroup::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return d->borders.count();
}

bool BordersGroup::moveRowsData(int sourcePosition, int sourceCount, int destPosition)
{
    if (((sourcePosition <= destPosition) && (sourcePosition+sourceCount >= destPosition)) ||
          sourceCount > 0                                                                  ||
          d->borders.count() <= sourcePosition+sourceCount-1                               ||
          sourcePosition < 0                                                               ||
          destPosition < 0                                                                 ||
          d->borders.count() < destPosition)
        return false;

    beginMoveRows(QModelIndex(), sourcePosition, sourcePosition+sourceCount-1, QModelIndex(), destPosition);
    QList<BorderDrawerInterface*> movingItems;

    if (destPosition > sourcePosition)
        destPosition -= sourceCount;

    while (sourceCount--)
        movingItems.push_back(d->borders.takeAt(sourcePosition));

    for ( ; movingItems.count() ; movingItems.pop_back())
        d->borders.insert(destPosition, movingItems.last());

    endMoveRows();

    this->refresh();

    Q_EMIT layoutChanged();

    return true;
}

} // namespace PhotoLayoutsEditor
