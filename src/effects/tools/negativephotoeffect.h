/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2011-11-29
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

#ifndef NEGATIVE_PHOTO_EFFECT_H
#define NEGATIVE_PHOTO_EFFECT_H

// Qt includes

#include <QImage>
#include <QRect>

// Local includes

#include "abstractphotoeffectinterface.h"

namespace PhotoLayoutsEditor
{

class StandardEffectsFactory;

class NegativePhotoEffect : public AbstractPhotoEffectInterface
{
    Q_OBJECT

public:

    explicit NegativePhotoEffect(StandardEffectsFactory* factory, QObject* parent = nullptr);

    QImage apply(const QImage& image) const override;
    QString name() const override;
    QString toString() const override;
    operator QString() const override;

private:

    static QImage negative(const QImage& image)
    {
        QImage result       = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        unsigned int pixels = result.width() * result.height();
        unsigned int* data = reinterpret_cast<unsigned int*>(result.bits());

        for (unsigned int i = 0; i < pixels; ++i)
            data[i] = qRgb(255-qRed(data[i]),255-qGreen(data[i]),255-qBlue(data[i]));

        return result;
    }
};
    
} // namespace PhotoLayoutsEditor

#endif // NEGATIVEPHOTOEFFECT_H
