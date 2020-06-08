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

#ifndef TEMPLATESVIEW_H
#define TEMPLATESVIEW_H

#include <QTableView>

namespace PhotoLayoutsEditor
{
    class TemplatesView : public QAbstractItemView
    {
            Q_OBJECT

//            int columns;
            mutable int idealWidth;
            mutable int idealHeight;
            mutable bool hashIsDirty;

        public:

            explicit TemplatesView(QWidget * parent = nullptr);

            void mousePressEvent(QMouseEvent * event) override;
            void updateGeometries() override;
            void resizeEvent(QResizeEvent*) override;
            void paintOutline(QPainter * painter, const QRectF &rectangle);
            void paintEvent(QPaintEvent*) override;
            QRegion visualRegionForSelection(const QItemSelection &selection) const override;
            void setSelection(const QRect &rect, QFlags<QItemSelectionModel::SelectionFlag> flags) override;
            void scrollContentsBy(int dx, int dy) override;
            int horizontalOffset() const override;
            int verticalOffset() const override;
            QModelIndex moveCursor( QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers) override;
            void rowsInserted(const QModelIndex & parent, int start, int end) override;
            void rowsAboutToBeRemoved(const QModelIndex & parent, int start, int end) override;
            using QAbstractItemView::dataChanged;
            void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
            QModelIndex indexAt(const QPoint &point_) const override;
            void scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint) override;
            bool isIndexHidden(const QModelIndex&) const override;
            QRectF viewportRectForRow(int row) const;
            QRect visualRect(const QModelIndex &index) const override;
            void calculateRectsIfNecessary() const;
            void setModel(QAbstractItemModel * model) override;

            QString selectedPath() const;

        Q_SIGNALS:

        public Q_SLOTS:

    };
}
#endif // TEMPLATESVIEW_H
