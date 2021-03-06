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

#include "undocommandeventfilter.h"

// Qt includes

#include <QtGlobal>
#include <QDebug>

// Local includes

#include "undocommandevent.h"
#include "progressevent.h"
#include "pleglobal.h"
#include "plewindow.h"

namespace PhotoLayoutsEditor
{

bool UndoCommandEventFilter::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == UndoCommandEvent::registeredEventType())
    {
        PLEWindow* const editor = qobject_cast<PLEWindow*>(watched);

        if (!editor)
            return false;

        //editor->undoCommandEvent(dynamic_cast<UndoCommandEvent*>(event));

        return true;
    }

    if (event->type() == ProgressEvent::registeredEventType())
    {
        PLEWindow* const editor = qobject_cast<PLEWindow*>(watched);

        if (!editor)
            return false;

        editor->progressEvent(dynamic_cast<ProgressEvent*>(event));

        return true;
    }

    return QObject::eventFilter(watched, event);
}

} // namespace PhotoLayoutsEditor
