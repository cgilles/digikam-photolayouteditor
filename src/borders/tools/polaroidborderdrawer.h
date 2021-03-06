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

#ifndef POLAROID_BORDER_DRAWER_H
#define POLAROID_BORDER_DRAWER_H

// Qt includes

#include <QColor>

// Local includes

#include "borderdrawerinterface.h"

namespace PhotoLayoutsEditor
{

class StandardBordersFactory;

class PolaroidBorderDrawer : public BorderDrawerInterface
{
    Q_OBJECT

private:

    int                                m_width;
    QString                            m_text;
    QColor                             m_color;
    QFont                              m_font;
    QPainterPath                       m_path;
    QRectF                             m_text_rect;

    static QHash<const char*, QString> m_properties;
    static int                         m_default_width;
    static QString                     m_default_text;
    static QColor                      m_default_color;
    static QFont                       m_default_font;

public:

    explicit PolaroidBorderDrawer(StandardBordersFactory* factory, QObject* parent = nullptr);

    QPainterPath path(const QPainterPath& path)                               override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option)     override;

    QString propertyName(const QMetaProperty& property) const                 override;

    QVariant propertyValue(const QString& propertyName) const                 override;

    void setPropertyValue(const QString& propertyName, const QVariant& value) override;

    QDomElement toSvg(QDomDocument& document) const                           override;

    QString name() const                                                      override;

    QString toString() const                                                  override;

    operator QString() const                                                  override;

    // ---

    Q_PROPERTY(int width READ width WRITE setWidth)

    int width() const
    {
        return m_width;
    }

    void setWidth(int width)
    {
        if (width > 0)
        {
            m_default_width = m_width = width;
            this->propertiesChanged();
        }
    }

    // ---

    Q_PROPERTY(QString text READ text WRITE setText)

    QString text() const
    {
        return m_text;
    }

    void setText(const QString& text)
    {
        m_text = text;
        this->propertiesChanged();
    }

    // ---

    Q_PROPERTY(QColor color READ color WRITE setColor)

    QColor color() const
    {
        return m_color;
    }

    void setColor(const QColor& color)
    {
        if (color.isValid())
        {
            m_default_color = m_color = color;
            this->propertiesChanged();
        }
    }

    // ---

    Q_PROPERTY(QFont font READ font WRITE setFont)

    QFont font() const
    {
        return m_font;
    }

    void setFont(const QFont& font)
    {
        m_default_font = m_font = font;
        this->propertiesChanged();
    }

    QVariant minimumValue(const QMetaProperty& property) override;
    QVariant maximumValue(const QMetaProperty& property) override;
    QVariant stepValue(const QMetaProperty& property)    override;

private:

    QString pathToSvg(const QPainterPath& path) const;
};

} // namespace PhotoLayoutsEditor

#endif // POLAROID_BORDER_DRAWER_H
