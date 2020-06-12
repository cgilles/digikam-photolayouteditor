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

#include "layersmodelitem.h"

// Qt includes

#include <QVariant>
#include <QIcon>
#include <QString>
#include <QDebug>

// Local includes

#include "layersmodel.h"
#include "abstractphoto.h"

namespace PhotoLayoutsEditor
{

LayersModelItem::LayersModelItem(AbstractPhoto* photo, LayersModelItem* parent, LayersModel* model)
    : parentItem(nullptr),
      itemPhoto(photo),
      itemModel(model)
{
    if (parent != this)
    {
        this->parentItem = parent;

        if (parent)
            parent->childItems.append(this);
    }
}

LayersModelItem::~LayersModelItem()
{
    foreach (LayersModelItem* const child, childItems)
        delete child;
}

void LayersModelItem::removeChild(LayersModelItem* child)
{
    if (child && child->parent() == this)
        child->setParent(nullptr);
}

LayersModelItem* LayersModelItem::parent() const
{
    return parentItem;
}

int LayersModelItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<LayersModelItem*>(this));

    return 0;
}

int LayersModelItem::columnCount() const
{
    return COLUMN_COUNT;
}

int LayersModelItem::childCount() const
{
    return childItems.count();
}

LayersModelItem* LayersModelItem::child(int row) const
{
    return childItems.value(row);
}

void LayersModelItem::setParent(LayersModelItem* parent)
{
    if (parent == this->parentItem)
        return;

    if (parentItem)
        parentItem->removeChild(this);

    this->parentItem = parent;

    if (parent && !parent->childItems.contains(this))
        parent->childItems.prepend(this);

    this->QObject::setParent(parent);
}

void LayersModelItem::setData(const QList<QVariant>& data)
{
    int i = 0;

    for (QList<QVariant>::const_iterator it = data.begin(); it != data.end(); ++it)
    {
        setData(*it, i);
        i++;
    }
}

bool LayersModelItem::setData(const QVariant& data, int type)
{
    if (type >= COLUMN_COUNT || !this->itemPhoto)
        return false;

    if (type == NameString)
        this->itemPhoto->setName(data.toString());

    return false;
}

void LayersModelItem::updateData()
{
    if (itemModel)
    {
        QModelIndex index = itemModel->findIndex( this->photo() );

        if (index.isValid())
            itemModel->updateModel(index, index);
    }
}

void LayersModelItem::setPhoto(AbstractPhoto* photo)
{
    if (itemPhoto)
        disconnect(itemPhoto, SIGNAL(updated()), this, nullptr);

    this->itemPhoto = photo;

    if (photo)
    {
        int newZValue = this->parent()->childCount()-this->parent()->childItems.indexOf(this);

        if (photo->zValue() != newZValue)
            qDebug() << "ZValue changed!"
                                         << (QGraphicsItem*)photo
                                         << "Current:" << photo->zValue()
                                         << "New:" << newZValue;

        photo->setZValue(newZValue);
        connect(photo, SIGNAL(changed()), this, SLOT(updateData()));
    }
}

AbstractPhoto* LayersModelItem::photo() const
{
    return this->itemPhoto;
}

QVariant LayersModelItem::data(int column) const
{
    if      (column == LayersModelItem::NameString)
        return itemPhoto ? itemPhoto->name() : QObject::tr("Layer");
    else if (column == LayersModelItem::Thumbnail)
        return itemPhoto ? itemPhoto->icon() : QIcon();
    else
        return QVariant();
}

QList<QVariant> LayersModelItem::data() const
{
    QList<QVariant> result;

    for (int i = 0; i < COLUMN_COUNT; ++i)
        result << data(i);

    return result;
}

bool LayersModelItem::insertChildren(int position, LayersModelItem* item)
{
    if (position < 0 || position > childItems.size())
        return false;

    childItems.insert(position, item);

    if (item != nullptr)
        item->setParent(this);

    this->refreshZValues();

    return true;
}

bool LayersModelItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    this->refreshZValues();

    return true;
}

bool LayersModelItem::moveChildren(int sourcePosition, int count, LayersModelItem* destParent, int destPosition)
{
    if (!count || (sourcePosition < destPosition && sourcePosition+count-1 >= destPosition) || !destParent)
        return false;

    QList<LayersModelItem*> movingItems;

    if (this == destParent && destPosition > sourcePosition)
        destPosition -= count;

    for ( ; count ; --count)
        movingItems.push_back(this->childItems.takeAt(sourcePosition));

    for ( ; movingItems.count() ; movingItems.pop_back())
        destParent->childItems.insert(destPosition, movingItems.last());

    this->refreshZValues();

    if (destParent != this)
        destParent->refreshZValues();

    return true;
}

void LayersModelItem::refreshZValues()
{
    int i = childItems.count();

    foreach (LayersModelItem* item, childItems)
    {
        AbstractPhoto* photo = item->photo();

        if (photo)
            photo->setZValue(i);

        --i;
    }
}

} // namespace PhotoLayoutsEditor
