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

#ifndef ABSTRACT_PHOTO_EFFECT_FACTORY_H
#define ABSTRACT_PHOTO_EFFECT_FACTORY_H

// Qt includes

#include <QObject>
#include <QDomDocument>
#include <QDomNodeList>

// Local includes

#include "abstractphotoeffectinterface.h"

namespace PhotoLayoutsEditor
{

class AbstractPhotoEffectFactory : public QObject
{
    Q_OBJECT

public:

    explicit AbstractPhotoEffectFactory(QObject* const parent = nullptr);
    ~AbstractPhotoEffectFactory() override;

    /** Returns effects instance.
     * \arg browser - as this argument you can set \class QtAbstractPropertyBrowser received from virtual
     * \fn propertyBrowser() method of this object.
     */
    virtual AbstractPhotoEffectInterface* getEffectInstance(const QString& name = QString()) = 0;

    /** Returns effect name.
     * This name is used be the user to identify effect. This name should be unique becouse effects are identified by this name.
     * Moreover, this name is used in UI to name effects.
     */
    virtual QString effectName() const                                                       = 0;
};

} // namespace PhotoLayoutsEditor

Q_DECLARE_INTERFACE(PhotoLayoutsEditor::AbstractPhotoEffectFactory,"pl.coder89.ple.AbstractPhotoEffectFactory/1.0")

#endif // ABSTRACT_PHOTO_EFFECT_FACTORY_H
