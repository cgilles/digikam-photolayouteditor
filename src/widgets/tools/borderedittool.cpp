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

#include "borderedittool.h"

// Qt includes

#include <QDebug>
#include <QGridLayout>
#include <QFormLayout>
#include <QLabel>
#include <QFont>
#include <QColorDialog>
#include <QMetaProperty>
#include <QPushButton>

// Local includes

#include "toolsdockwidget.h"
#include "abstractphoto.h"
#include "bordersgroup.h"
#include "borderdrawersloader.h"
#include "pleeditfactory.h"
#include "qttreepropertybrowser.h"

namespace PhotoLayoutsEditor
{

BorderEditTool::BorderEditTool(PLEScene * scene, QWidget* parent) :
    AbstractItemsListViewTool(QObject::tr("Borders editor"), scene, PLECanvas::SingleSelcting, parent)
{
}

QStringList BorderEditTool::options() const
{
    return BorderDrawersLoader::registeredDrawers();
}

AbstractMovableModel * BorderEditTool::model()
{
    if (currentItem() && currentItem()->bordersGroup())
        return currentItem()->bordersGroup();
    return nullptr;
}

QObject* BorderEditTool::createItem(const QString& name)
{
    return BorderDrawersLoader::getDrawerByName(name);
}

QWidget* BorderEditTool::createEditor(QObject* item, bool createCommands)
{
    BorderDrawerInterface* drawer = qobject_cast<BorderDrawerInterface*>(item);
    if (!drawer)
        return nullptr;
    return BorderDrawersLoader::createEditor(drawer, createCommands);
}

} // namespace PhotoLayoutsEditor
