/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2011-09-01
 * Description : a plugin to create photo layouts by fusion of several images.
 *
 * Copyright (C) 2011      by Lukasz Spas <lukasz dot spas at gmail dot com>
 * Copyright (C) 2009-2020 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef SOLIDBORDERDRAWER_H
#define SOLIDBORDERDRAWER_H

#include "BorderDrawerInterface.h"

#include <QColor>

namespace PhotoLayoutsEditor
{
    class StandardBordersFactory;

    class SolidBorderDrawer : public BorderDrawerInterface
    {
            Q_OBJECT

            int m_width;
            QColor m_color;
            int m_spacing;
            Qt::PenJoinStyle m_corners_style;
            QPainterPath m_path;

            static QMap<const char *,QString> m_properties;
            static QMap<Qt::PenJoinStyle, QString> m_corners_style_names;
            static int m_default_width;
            static QColor m_default_color;
            static int m_default_spacing;
            static Qt::PenJoinStyle m_default_corners_style;

        public:

            explicit SolidBorderDrawer(StandardBordersFactory * factory, QObject * parent = nullptr);

            virtual QPainterPath path(const QPainterPath & path) override;

            virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option) override;

            virtual QString propertyName(const QMetaProperty & property) const override;

            virtual QVariant propertyValue(const QString & propertyName) const override;

            virtual void setPropertyValue(const QString & propertyName, const QVariant & value) override;

            virtual QDomElement toSvg(QDomDocument & document) const override;

            virtual QString name() const override;

            virtual QString toString() const override;

            virtual operator QString() const override;

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

            Q_PROPERTY(QString corners_style READ cornersStyle WRITE setCornersStyle)
            QString cornersStyle() const
            {
                return m_corners_style_names.value(m_corners_style);
            }
            void setCornersStyle(const QString & cornersStyle)
            {
                m_default_corners_style = m_corners_style = m_corners_style_names.key(cornersStyle);
                this->propertiesChanged();
            }

            Q_PROPERTY(QColor color READ color WRITE setColor)
            QColor color() const
            {
                return m_color;
            }
            void setColor(const QColor & color)
            {
                if (color.isValid())
                {
                    m_default_color = m_color = color;
                    this->propertiesChanged();
                }
            }

            Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
            int spacing() const
            {
                return m_spacing;
            }
            void setSpacing(int spacing)
            {
                m_default_spacing = m_spacing = spacing;
                this->propertiesChanged();
            }

            virtual QVariant stringNames(const QMetaProperty & property) override;
            virtual QVariant minimumValue(const QMetaProperty & property) override;
            virtual QVariant maximumValue(const QMetaProperty & property) override;
            virtual QVariant stepValue(const QMetaProperty & property) override;
    };
}

#endif // SOLIDBORDERDRAWER_H
