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

#ifndef BORDER_EDIT_TOOL_H
#define BORDER_EDIT_TOOL_H

// Local includes

#include "abstractitemslistviewtool.h"

// Qt includes

#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>

namespace PhotoLayoutsEditor
{

class BorderEditTool : public AbstractItemsListViewTool
{
    Q_OBJECT

public:

    explicit BorderEditTool(PLEScene* scene, QWidget* parent = nullptr);

    virtual QStringList options() const                                      override;
    virtual AbstractMovableModel* model()                                    override;
    virtual QObject* createItem(const QString& name)                         override;
    virtual QWidget* createEditor(QObject* item, bool createCommands = true) override;
};

} // namespace PhotoLayoutsEditor

#endif // BORDER_EDIT_TOOL_H
