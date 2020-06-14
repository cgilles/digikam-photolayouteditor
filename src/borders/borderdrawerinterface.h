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

#ifndef BORDER_DRAWER_INTERFACE_H
#define BORDER_DRAWER_INTERFACE_H

// Qt includes

#include <QObject>
#include <QDomDocument>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QCoreApplication>

namespace PhotoLayoutsEditor
{

class BordersGroup;
class BorderDrawerFactoryInterface;

class BorderDrawerInterface : public QObject
{
    Q_OBJECT

public:

    explicit BorderDrawerInterface(BorderDrawerFactoryInterface* factory, QObject* parent = nullptr)
        : QObject(parent),
          m_group(nullptr),
          m_factory(factory)
    {
    }

    void setGroup(BordersGroup* group)
    {
        this->m_group = group;
    }

    BordersGroup* group() const
    {
        return this->m_group;
    }

    BorderDrawerFactoryInterface* factory() const
    {
        return m_factory;
    }

public:

    virtual QPainterPath path(const QPainterPath& path)                               = 0;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option)     = 0;

    virtual QString propertyName(const QMetaProperty& property)                 const = 0;
    virtual QVariant propertyValue(const QString& propertyName)                 const = 0;
    virtual void setPropertyValue(const QString& propertyName, const QVariant& value) = 0;

    virtual QVariant stringNames(const QMetaProperty& /*property*/) { return QVariant(); }
    virtual QVariant minimumValue(const QMetaProperty& /*property*/){ return QVariant(); }
    virtual QVariant maximumValue(const QMetaProperty& /*property*/){ return QVariant(); }
    virtual QVariant stepValue(const QMetaProperty& /*property*/)   { return QVariant(); }

    virtual QDomElement toSvg(QDomDocument& document) const = 0;
    virtual QString name()                            const = 0;
    virtual QString toString()                        const = 0;
    virtual operator QString()                        const = 0;

public:

    BordersGroup*                 m_group;
    BorderDrawerFactoryInterface* m_factory;

Q_SIGNALS:

    void changed();

protected:

    void propertiesChanged()
    {
        Q_EMIT changed();
    }
};

} // namespace PhotoLayoutsEditor

#endif // BORDER_DRAWER_INTERFACE_H
