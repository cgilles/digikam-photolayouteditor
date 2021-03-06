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

#ifndef CANVAS_LOADING_THREAD_H
#define CANVAS_LOADING_THREAD_H

// Qt includes

#include <QThread>
#include <QDomDocument>

// Local includes

#include "progressobserver.h"

namespace PhotoLayoutsEditor
{

class AbstractPhoto;
class PLESceneBackground;
class PLESceneBorder;

class PLECanvasLoadingThread : public QThread, public ProgressObserver
{
    Q_OBJECT

public:

    explicit PLECanvasLoadingThread(QObject* const parent = nullptr);
    ~PLECanvasLoadingThread() override;

    void progresChanged(double progress) override;
    void progresName(const QString& name) override;
    void addItem(AbstractPhoto* item, QDomElement& element);
    void addBackground(PLESceneBackground* background, QDomElement& element);
    void addBorder(PLESceneBorder* border, QDomElement& element);

protected:

    void run() override;

private:

    PLECanvasLoadingThread(const PLECanvasLoadingThread&);
    PLECanvasLoadingThread& operator=(const PLECanvasLoadingThread&);

    class PLECanvasLoadingThreadPrivate;
    PLECanvasLoadingThreadPrivate* d;

    friend class PLECanvasLoadingThreadPrivate;
};

} // namespace PhotoLayoutsEditor

#endif // CANVAS_LOADING_THREAD_H
