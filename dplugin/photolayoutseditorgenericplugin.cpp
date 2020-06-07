/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
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
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "photolayoutseditorgenericplugin.h"

// Qt includes

#include <QApplication>
#include <QList>
#include <QUrl>

// digiKam includes

#include <dinfointerface.h>

// Local includes

#include "plewindow.h"

using namespace PhotoLayoutsEditor;

namespace DigikamGenericPhotoLayoutsEditorPlugin
{

PhotoLayoutsEditorPlugin::PhotoLayoutsEditorPlugin(QObject* const parent)
    : DPluginGeneric(parent)
{
}

PhotoLayoutsEditorPlugin::~PhotoLayoutsEditorPlugin()
{
}

QString PhotoLayoutsEditorPlugin::name() const
{
    return QString::fromUtf8("Photo Layouts Editor");
}

QString PhotoLayoutsEditorPlugin::iid() const
{
    return QLatin1String(DPLUGIN_IID);
}

QIcon PhotoLayoutsEditorPlugin::icon() const
{
    return QIcon(QLatin1String(":/photolayoutseditor.png"));
}

QString PhotoLayoutsEditorPlugin::description() const
{
    return QString::fromUtf8("A tool to create collage layout of images using effect.");
}

QString PhotoLayoutsEditorPlugin::details() const
{
    return QString::fromUtf8("<p>This tool allows you to create layouts of photo based on a fusion of several images.</p>");
}

QList<DPluginAuthor> PhotoLayoutsEditorPlugin::authors() const
{
    return QList<DPluginAuthor>()
            << DPluginAuthor(QString::fromUtf8("Lukasz Spas"),
                             QString::fromUtf8("lukasz dot spas at gmail dot com>e"),
                             QString::fromUtf8("(C) 2011-2012"))
            << DPluginAuthor(QString::fromUtf8("Gilles Caulier"),
                             QString::fromUtf8("caulier dot gilles at gmail dot com"),
                             QString::fromUtf8("(C) 2011-2020"))
    ;
}

void PhotoLayoutsEditorPlugin::setup(QObject* const parent)
{
    DPluginAction* const ac = new DPluginAction(parent);
    ac->setIcon(icon());
    ac->setText(QString::fromUtf8("Photo Layouts Editor..."));
    ac->setObjectName(QLatin1String("photolayoutseditor"));
    ac->setActionCategory(DPluginAction::GenericTool);

    connect(ac, SIGNAL(triggered(bool)),
            this, SLOT(slotPhotoLayoutsEditor()));

    addAction(ac);
}

void PhotoLayoutsEditorPlugin::slotPhotoLayoutsEditor()
{
    DInfoInterface* const iface = infoIface(sender());
    QList<QUrl> images          = iface->currentSelectedItems();

    if (images.isEmpty())
    {
        images = iface->currentAlbumItems();
    }

    if (!images.isEmpty())
    {
        PLEWindow* const w = PLEWindow::instance(this);
        w->setInterface(iface);
        w->openFile();
        w->loadImages(images);
        w->show();
    }
}

} // namespace DigikamGenericPhotoLayoutsEditorPlugin
