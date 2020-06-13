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

#include "standardeffectsfactory.h"

// Local includes

#include "blurphotoeffect.h"
#include "colorizephotoeffect.h"
#include "grayscalephotoeffect.h"
#include "sepiaphotoeffect.h"
#include "negativephotoeffect.h"

namespace PhotoLayoutsEditor
{

StandardEffectsFactory::StandardEffectsFactory(QObject* parent)
    : AbstractPhotoEffectFactory(parent)
{
}

AbstractPhotoEffectInterface* StandardEffectsFactory::getEffectInstance(const QString& name)
{
    if (name == QObject::tr("Blur effect"))
        return new BlurPhotoEffect(this);

    if (name == QObject::tr("Colorize effect"))
        return new ColorizePhotoEffect(this);

    if (name == QObject::tr("Grayscale effect"))
        return new GrayscalePhotoEffect(this);

    if (name == QObject::tr("Sepia effect"))
        return new SepiaPhotoEffect(this);

    if (name == QObject::tr("Negative effect"))
        return new NegativePhotoEffect(this);

    return nullptr;
}

QString StandardEffectsFactory::effectName() const
{
    return QObject::tr("Blur effect") + QLatin1String(";") +
           QObject::tr("Colorize effect") + QLatin1String(";") +
           QObject::tr("Grayscale effect") + QLatin1String(";") +
           QObject::tr("Sepia effect") + QLatin1String(";") +
           QObject::tr("Negative effect");
}

} // namespace PhotoLayoutsEditor
