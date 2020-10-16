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

#include "photoeffectsgroup.h"

// Local includes

#include "photoeffectsloader.h"
#include "abstractphoto.h"
#include "abstractphotoeffectfactory.h"

namespace PhotoLayoutsEditor
{

class PhotoEffectsGroupPrivate
{
    explicit PhotoEffectsGroupPrivate(PhotoEffectsGroup* group)
        : group(group),
          photo(nullptr)
    {
    }

    PhotoEffectsGroup*                   group;
    AbstractPhoto*                       photo;
    QList<AbstractPhotoEffectInterface*> effects;

    friend class PhotoEffectsGroup;
};

PhotoEffectsGroup::PhotoEffectsGroup(AbstractPhoto* const photo)
    : AbstractMovableModel(photo),
      d(new PhotoEffectsGroupPrivate(this))
{
    d->photo = photo;

    connect(this, SIGNAL(effectsChanged()), photo, SLOT(refresh()));
}

PhotoEffectsGroup::~PhotoEffectsGroup()
{
    qDebug() << "PhotoEffectsGroup delete";
    delete d;
}

QDomElement PhotoEffectsGroup::toSvg(QDomDocument& document) const
{
    QDomElement effectsGroup = document.createElement(QLatin1String("effects"));

    for (int i = d->effects.count()-1; i >= 0; --i)
    {
        QDomElement e = PhotoEffectsLoader::effectToSvg(d->effects[i], document);

        if (e.isNull())
            continue;

        effectsGroup.appendChild(e);
    }

    return effectsGroup;
}

PhotoEffectsGroup* PhotoEffectsGroup::fromSvg(const QDomElement& element, AbstractPhoto* graphicsItem)
{
    QDomElement temp = element;

    if (temp.tagName() != QLatin1String("effects"))
        temp = temp.firstChildElement(QLatin1String("effects"));

    if (temp.isNull())
        return nullptr;

    PhotoEffectsGroup* group = new PhotoEffectsGroup(graphicsItem);
    QDomNodeList effectsList = temp.childNodes();

    for (int i = effectsList.count()-1; i >= 0; --i)
    {
        QDomElement effect = effectsList.at(i).toElement();

        if (effect.isNull())
            continue;

        AbstractPhotoEffectInterface* interface = PhotoEffectsLoader::getEffectFromSvg(effect);

        if (interface)
            group->push_back(interface);
    }

    return group;
}

void PhotoEffectsGroup::push_back(AbstractPhotoEffectInterface* effect)
{
    d->effects.push_back(effect);

    connect(effect, SIGNAL(changed()),
            this, SLOT(emitEffectsChanged()));

    effect->setParent(this);
    effect->setGroup(this);
    Q_EMIT layoutChanged();
}

void PhotoEffectsGroup::push_front(AbstractPhotoEffectInterface* effect)
{
    d->effects.push_front(effect);

    connect(effect, SIGNAL(changed()),
            this, SLOT(emitEffectsChanged()));

    effect->setParent(this);
    effect->setGroup(this);
    Q_EMIT layoutChanged();
}

QImage PhotoEffectsGroup::apply(const QImage& image)
{
    QImage temp = image;

    for (int i = d->effects.count()-1; i >= 0; --i)
    {
        AbstractPhotoEffectInterface* effect = d->effects[i];

        if (effect)
            temp = effect->apply(temp);
    }

    return temp;
}

AbstractPhoto* PhotoEffectsGroup::photo() const
{
    return d->photo;
}

QObject* PhotoEffectsGroup::item(const QModelIndex& index) const
{
    if (index.isValid() && index.row() < rowCount())
        return d->effects.at(index.row());

    return nullptr;
}

void PhotoEffectsGroup::setItem(QObject* item, const QModelIndex& index)
{
    AbstractPhotoEffectInterface* effect = dynamic_cast<AbstractPhotoEffectInterface*>(item);

    if (!effect || !index.isValid())
        return;

    int row = index.row();

    if (row < 0 || row >= rowCount())
        return;

    if (effect == d->effects.at(row))
        return;

    AbstractPhotoEffectInterface* temp = d->effects.takeAt(row);

    if (temp)
        temp->disconnect(this);

    d->effects.insert(row, effect);

    connect(effect, SIGNAL(changed()),
            this, SLOT(emitEffectsChanged()));

    emitEffectsChanged(effect);
}

AbstractPhotoEffectInterface* PhotoEffectsGroup::graphicsItem(const QModelIndex& index) const
{
    return static_cast<AbstractPhotoEffectInterface*>(index.internalPointer());
}

bool PhotoEffectsGroup::moveRowsData(int sourcePosition, int sourceCount, int destPosition)
{
    if ((sourcePosition <= destPosition && sourcePosition+sourceCount >= destPosition) ||
         sourceCount <= 0                                                              ||
         d->effects.count() <= sourcePosition+sourceCount-1                            ||
         sourcePosition < 0                                                            ||
         destPosition < 0                                                              ||
         d->effects.count() < destPosition)
        return false;

    beginMoveRows(QModelIndex(), sourcePosition, sourcePosition+sourceCount-1, QModelIndex(), destPosition);
    QList<AbstractPhotoEffectInterface*> movingItems;

    if (destPosition > sourcePosition)
        destPosition -= sourceCount;

    while (sourceCount--)
        movingItems.push_back(d->effects.takeAt(sourcePosition));

    for ( ; movingItems.count() ; movingItems.pop_back())
        d->effects.insert(destPosition, movingItems.last());

    endMoveRows();
    this->emitEffectsChanged();
    Q_EMIT layoutChanged();

    return true;
}

int PhotoEffectsGroup::columnCount(const QModelIndex& /*parent*/) const
{
    return 1;
}

QVariant PhotoEffectsGroup::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (!index.isValid())
    {
        return QObject::tr("Effect name");
    }
    else
    {
        AbstractPhotoEffectInterface* effect = graphicsItem(index);

        if (effect)
            return effect->toString();
        else
            return QVariant();
    }
}

Qt::ItemFlags PhotoEffectsGroup::flags(const QModelIndex& index) const
{
    Qt::ItemFlags result = QAbstractItemModel::flags(index);

    if (index.isValid() && !index.internalPointer())
        result |= Qt::ItemIsEditable;

    return result;
}

QModelIndex PhotoEffectsGroup::index(int row, int column, const QModelIndex& parent) const
{
    if (column != 0)
        return QModelIndex();

    if (row >= d->effects.count())
        return QModelIndex();

    if (parent.isValid())
        return QModelIndex();

    return createIndex(row,column,d->effects.at(row));
}

bool PhotoEffectsGroup::insertRows(int row, int count, const QModelIndex& parent)
{
    if (row < 0 || row > rowCount() || count < 1 || parent.isValid())
        return false;

    beginInsertRows(parent, row, row+count-1);

    while (count--)
        d->effects.insert(row, nullptr);

    endInsertRows();
    Q_EMIT layoutChanged();
    return true;
}

QModelIndex PhotoEffectsGroup::parent(const QModelIndex& /*index*/) const
{
    return QModelIndex();
}

int PhotoEffectsGroup::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return this->d->effects.count();
    else
        return 0;
}

bool PhotoEffectsGroup::removeRows(int row, int count, const QModelIndex& parent)
{
    if (count <= 0 || parent.isValid() || row < 0 || row >= rowCount(parent) || row+count > rowCount(parent))
        return false;

    beginRemoveRows(QModelIndex(), row, row+count-1);

    while (count--)
        d->effects.removeAt(row);

    endRemoveRows();
    this->emitEffectsChanged();
    Q_EMIT layoutChanged();
    return true;
}

void PhotoEffectsGroup::emitEffectsChanged(AbstractPhotoEffectInterface* effect)
{
    if (!d->photo)
        return;

    d->photo->refresh();

    if      (effect)
    {
        int row = d->effects.indexOf(effect);
        QModelIndex indexChanged = index(row,0);
        Q_EMIT dataChanged(indexChanged,indexChanged);
    }
    else if (rowCount())
    {
        Q_EMIT dataChanged(index(0,0),index(rowCount()-1,0));
    }

    Q_EMIT effectsChanged();
}

} // namespace PhotoLayoutsEditor
