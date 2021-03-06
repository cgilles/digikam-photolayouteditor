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

#include "standardbordersfactory.h"

// Local includes

#include "polaroidborderdrawer.h"
#include "solidborderdrawer.h"

namespace PhotoLayoutsEditor
{

StandardBordersFactory::StandardBordersFactory(QObject* const parent)
    : BorderDrawerFactoryInterface(parent)
{
}

QString StandardBordersFactory::drawersNames() const
{
    return QObject::tr("Polaroid border") + QLatin1String(";") +
           QObject::tr("Solid border");
}

BorderDrawerInterface* StandardBordersFactory::getDrawerInstance(const QString& name)
{
    if (name == QObject::tr("Solid border"))
       return new SolidBorderDrawer(this);

    if (name == QObject::tr("Polaroid border"))
       return new PolaroidBorderDrawer(this);

    return nullptr;
}

} // namespace PhotoLayoutsEditor
