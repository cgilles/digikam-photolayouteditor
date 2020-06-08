/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2011-09-01
 * Description : a plugin to create photo layouts by fusion of several images.
 *
 * Copyright (C) 2011-2012 by Lukasz Spas <lukasz dot spas at gmail dot com>
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

#ifndef CANVAS_SIZE_WIDGET_H
#define CANVAS_SIZE_WIDGET_H

// Qt includes

#include <QWidget>

namespace PhotoLayoutsEditor
{
    class PLECanvasSize;
    class PLECanvasSizeWidget : public QWidget
    {
            Q_OBJECT

        public:

            enum Orientation
            {
                Horizontal,
                Vertical
            };

            explicit PLECanvasSizeWidget(QWidget* parent = nullptr);
            ~PLECanvasSizeWidget();
            Orientation orientation() const;
            PLECanvasSize canvasSize() const;

        Q_SIGNALS:

            void orientationChanged();

        public Q_SLOTS:

            void sizeUnitsChanged(const QString& unitName);
            void resolutionUnitsChanged(const QString& unitName);
            void setHorizontal(bool isHorizontal);
            void setVertical(bool isVertical);
            void widthChanged(double width);
            void heightChanged(double height);
            void xResolutionChanged(double xResolution);
            void yResolutionChanged(double yResolution);

        private:

            void setupUI(const QSizeF & size, const QString& sizeUnits, const QSizeF & resolution, const QString& resolutionUnits);

            void prepareSignalsConnections();

            class Private;
            Private* d;
            friend class Private;
    };

} // namespace PhotoLayoutsEditor

#endif // CANVAS_SIZE_WIDGET_H
