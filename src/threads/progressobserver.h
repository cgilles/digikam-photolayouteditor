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

#ifndef PROGRESS_OBSERVER_H
#define PROGRESS_OBSERVER_H

// Qt includes

#include <QString>

namespace PhotoLayoutsEditor
{

class ProgressObserver
{

public:

    explicit ProgressObserver(ProgressObserver* parent = nullptr)
        : parent(parent)
    {
    }

    virtual ~ProgressObserver() = default;

    virtual void progresChanged(double progress)
    {
        if (parent)
            parent->progresChanged(progress);
    }

    virtual void progresName(const QString& name)
    {
        if (parent)
            parent->progresName(name);
    }

private:

    ProgressObserver* parent;
};

} // namespace PhotoLayoutsEditor

#endif // PROGRESS_OBSERVER_H
