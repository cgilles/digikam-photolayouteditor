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

#ifndef BORDER_DRAWERS_LOADER_H
#define BORDER_DRAWERS_LOADER_H

// Qt includes

#include <QMap>
#include <QObject>
#include <QDomDocument>

// Local includes

#include "borderdrawerfactoryinterface.h"

class QtProperty;

namespace PhotoLayoutsEditor
{

class BorderDrawerFactoryInterface;

class BorderDrawersLoaderPrivate;

class BorderDrawersLoader : public QObject
{
    Q_OBJECT

    BorderDrawersLoaderPrivate* d;
    static BorderDrawersLoader* m_instance;

public:

    ~BorderDrawersLoader();
    static BorderDrawersLoader* instance(QObject* parent = nullptr);
    static void registerDrawer(BorderDrawerFactoryInterface* factory);
    static QStringList registeredDrawers();
    static BorderDrawerFactoryInterface* getFactoryByName(const QString& name);
    static BorderDrawerInterface* getDrawerByName(const QString& name);
    static BorderDrawerInterface* getDrawerFromSvg(QDomElement& drawerElement);
    static QDomElement drawerToSvg(BorderDrawerInterface* drawer, QDomDocument& document);
    static QWidget* createEditor(BorderDrawerInterface* drawer, bool createCommands);

private:

    explicit BorderDrawersLoader(QObject* const parent = nullptr);
};

} // namespace PhotoLayoutsEditor

#endif // BORDER_DRAWERS_LOADER_H
