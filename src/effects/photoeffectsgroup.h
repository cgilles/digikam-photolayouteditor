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

// Local includes

#include "abstractmovablemodel.h"

class QDomDocument;
class QDomElement;

namespace PhotoLayoutsEditor
{

class AbstractPhoto;
class PhotoEffectsLoader;
class AbstractPhotoEffectInterface;

class PhotoEffectsGroup : public AbstractMovableModel
{
    Q_OBJECT

public:

    explicit PhotoEffectsGroup(AbstractPhoto* const photo);
    ~PhotoEffectsGroup() override;

    QDomElement toSvg(QDomDocument& document) const;
    static PhotoEffectsGroup* fromSvg(const QDomElement& element, AbstractPhoto* graphicsItem);
    AbstractPhoto* photo() const;
    AbstractPhotoEffectInterface* graphicsItem(const QModelIndex& index = QModelIndex()) const;
    bool insertRow(int row, AbstractPhotoEffectInterface* effect);
    bool insertRow(int row, const QModelIndex& index = QModelIndex());

    // Implement AbstractMovableModel methods
    bool moveRowsData(int sourcePosition, int sourceCount, int destPosition) override;
    void setItem(QObject* graphicsItem, const QModelIndex& index) override;
    QObject* item(const QModelIndex& index) const override;

    // Reimplemented QAbstractItemModel methods
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    bool removeRows(int row, int count, const QModelIndex& parent) override;

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
