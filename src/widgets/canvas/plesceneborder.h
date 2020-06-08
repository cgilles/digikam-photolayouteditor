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

#ifndef SCENEBORDER_H
#define SCENEBORDER_H

#include <QBrush>
#include <QGraphicsItem>
#include <QDomDocument>

namespace PhotoLayoutsEditor
{
    class PLESceneBorderLoader;

    class PLESceneBorder : public QObject, public QGraphicsItem
    {
            Q_OBJECT
            Q_INTERFACES(QGraphicsItem)

            QRectF m_rect;

            // Image border specific data
            QImage m_image;

            // For painting/rendering purpose
            QImage m_temp_image;

            class BorderImageChangedCommand;
            class BorderFirstBrushChangeCommand;
            class BorderSecondBrushChangeCommand;

        public:

            explicit PLESceneBorder(QGraphicsScene * scene = nullptr);
            virtual QRectF boundingRect() const override;

            void setImage(const QImage & image);

            QDomElement toSvg(QDomDocument & document) const;
            bool fromSvg(QDomElement & element);

            QImage image() const;
            QSize imageSize() const;

        Q_SIGNALS:

            void changed();

        protected:

            QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;
            void render(QPainter * painter, const QRect & rect);

        protected Q_SLOTS:

            void render();

        private:

            void sceneChanged();

        private Q_SLOTS:

            void sceneRectChanged(const QRectF & sceneRect);

        friend class PLESceneBorderLoader;
        friend class BorderImageChangedCommand;
        friend class BorderFirstBrushChangeCommand;
        friend class BorderSecondBrushChangeCommand;
    };
}

#endif // SCENEBORDER_H
