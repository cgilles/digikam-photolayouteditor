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

#include "effectseditortool.h"

// Qt includes

#include <QGridLayout>
#include <QLabel>
#include <QListView>
#include <QPushButton>
#include <QComboBox>

// Local includes

#include "qtpropertybrowser.h"
#include "abstractphoto.h"
#include "photoeffectsloader.h"
#include "photoeffectsgroup.h"
#include "abstractphotoeffectinterface.h"
#include "toolsdockwidget.h"

namespace PhotoLayoutsEditor
{

EffectsEditorTool::EffectsEditorTool(PLEScene * scene, QWidget* parent) :
    AbstractItemsListViewTool(QObject::tr("Effects"), scene, PLECanvas::SingleSelcting, parent)
{
}

QStringList EffectsEditorTool::options() const
{
    return PhotoEffectsLoader::registeredEffectsNames();
}

AbstractMovableModel * EffectsEditorTool::model()
{
    if (currentItem() && currentItem()->effectsGroup())
        return currentItem()->effectsGroup();
    return nullptr;
}

QObject* EffectsEditorTool::createItem(const QString& name)
{
    return PhotoEffectsLoader::getEffectByName(name);
}

QWidget* EffectsEditorTool::createEditor(QObject* item, bool createCommands)
{
    AbstractPhotoEffectInterface * effect = qobject_cast<AbstractPhotoEffectInterface*>(item);
    if (!effect)
        return nullptr;
    return PhotoEffectsLoader::propertyBrowser(effect, createCommands);
}

} // namespace PhotoLayoutsEditor
