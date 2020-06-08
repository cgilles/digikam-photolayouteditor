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

#ifndef PLE_CANVAS_P_H
#define PLE_CANVAS_P_H

#include "plecanvas.h"

// Qt includes

#include <QMap>
#include <QProgressBar>

// Local includes

#include "plecanvassize.h"

namespace PhotoLayoutsEditor
{

class PLECanvasSavingThread;

class PLECanvasPrivate
{
public:

    PLECanvasPrivate()
        : m_template(false)
    {
    }

    PLECanvasSize                    m_size;
    bool                          m_template;
    QMap<QObject*, QProgressBar*> progressMap;

public:

    friend class PLECanvas;
    friend class PLECanvasSavingThread;
};

} // namespace PhotoLayoutsEditor

#endif // PLE_CANVAS_P_H
