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

#ifndef TEXT_COLOR_CHANGE_LISTENER_H
#define TEXT_COLOR_CHANGE_LISTENER_H

// Qt includes

#include <QObject>

class QtProperty;

namespace PhotoLayoutsEditor
{

class TextItem;

class TextColorChangeListener : public QObject
{
    Q_OBJECT

    TextItem* m_item;

public:

    explicit TextColorChangeListener(TextItem* item, QObject* parent = nullptr);

public Q_SLOTS:

    void propertyChanged(QtProperty* property);
};

} // namespace PhotoLayoutsEditor

#endif // TEXT_COLOR_CHANGE_LISTENER_H
