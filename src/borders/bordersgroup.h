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

#ifndef BORDERS_GROUP_H
#define BORDERS_GROUP_H

#include "abstractmovablemodel.h"

// Qt includes

#include <QPainter>
#include <QPainterPath>
#include <QDomDocument>
#include <QStyleOptionGraphicsItem>

// Local includes

#include "borderdrawerinterface.h"

namespace PhotoLayoutsEditor
{

class BordersGroupPrivate;
class AbstractPhoto;

class BordersGroup : public AbstractMovableModel
{
    Q_OBJECT

    BordersGroupPrivate* d;

public:

    explicit BordersGroup(AbstractPhoto* const graphicsItem);
    ~BordersGroup();

    QPainterPath shape();
    AbstractPhoto* graphicsItem() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option);

    // Method used for model manipulation
    bool prependDrawer(BorderDrawerInterface* drawer);
    bool insertDrawer(BorderDrawerInterface* drawer, int position);
    bool appendDrawer(BorderDrawerInterface* drawer);
    BorderDrawerInterface* removeDrawer(int position);
    bool moveDrawer(int sourcePosition, int destinationPosition);

    QDomElement toSvg(QDomDocument& document);

    static BordersGroup* fromSvg(QDomElement& element, AbstractPhoto* graphicsItem);

Q_SIGNALS:

    void drawersChanged();

public Q_SLOTS:

    void refresh();

protected:

    virtual QObject* item(const QModelIndex& index)                                           const override;
    virtual void setItem(QObject* graphicsItem, const QModelIndex& index)                           override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex())                        const override;
    virtual QVariant data(const QModelIndex& index, int role)                                 const override;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex())          override;
    virtual QModelIndex parent(const QModelIndex& child)                                      const override;
    virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex())          override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex())                           const override;
    virtual bool moveRowsData(int sourcePosition, int sourceCount, int destPosition)                override;

protected Q_SLOTS:

    void emitBordersChanged()
    {
        Q_EMIT drawersChanged();
    }

private:

    void calculateShape();
    Q_DISABLE_COPY(BordersGroup)
};

} // namespace PhotoLayoutsEditor

#endif // BORDERS_GROUP_H
