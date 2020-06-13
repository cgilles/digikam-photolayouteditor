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

#ifndef PHOTOEFFECTSGROUP_H
#define PHOTOEFFECTSGROUP_H

// Qt includes

#include <QPixmap>
#include <QDomDocument>

// Local includes

#include "abstractmovablemodel.h"

namespace PhotoLayoutsEditor
{

class AbstractPhoto;
class PhotoEffectsLoader;
class AbstractPhotoEffectInterface;

class PhotoEffectsGroup : public AbstractMovableModel
{
    Q_OBJECT

public:

    explicit PhotoEffectsGroup(AbstractPhoto* photo, QObject* parent = nullptr);
    ~PhotoEffectsGroup();

    QDomElement toSvg(QDomDocument& document) const;
    static PhotoEffectsGroup* fromSvg(const QDomElement& element, AbstractPhoto* graphicsItem);
    AbstractPhoto* photo() const;
    virtual QObject* item(const QModelIndex& index) const override;
    virtual void setItem(QObject* graphicsItem, const QModelIndex& index) override;
    AbstractPhotoEffectInterface* graphicsItem(const QModelIndex& index = QModelIndex()) const;
    bool moveRowsData(int sourcePosition, int sourceCount, int destPosition) override;
    bool insertRow(int row, AbstractPhotoEffectInterface* effect);
    bool insertRow(int row, const QModelIndex& index = QModelIndex());

    // Reimplemented QAbstractItemModel methods
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    virtual QModelIndex parent(const QModelIndex& index) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool removeRows(int row, int count, const QModelIndex& parent) override;

Q_SIGNALS:

    void effectsChanged();

public Q_SLOTS:

    void push_back(AbstractPhotoEffectInterface* effect);
    void push_front(AbstractPhotoEffectInterface* effect);
    void emitEffectsChanged(AbstractPhotoEffectInterface* effect = nullptr);
    QImage apply(const QImage& image);

public:

    AbstractPhoto*                       m_photo;
    QList<AbstractPhotoEffectInterface*> m_effects_list;

    friend class AbstractPhoto;
};

} // namespace PhotoLayoutsEditor

#endif // PHOTOEFFECTSGROUP_H
