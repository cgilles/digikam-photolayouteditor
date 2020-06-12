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

#include "plecanvasloadingthread.h"

// Qt includes

#include <QCoreApplication>

// Local includes

#include "progressevent.h"
#include "abstractphotoitemloader.h"
#include "photoitemloader.h"
#include "textitemloader.h"
#include "plescenebackgroundloader.h"
#include "plesceneborderloader.h"
#include "abstractphoto.h"
#include "photoitem.h"
#include "textitem.h"
#include "plescenebackground.h"
#include "plesceneborder.h"
#include "plewindow.h"

namespace PhotoLayoutsEditor
{

class PLECanvasLoadingThread::PLECanvasLoadingThreadPrivate
{
    PLECanvasLoadingThreadPrivate()
    {
        i     = 0;
        count = 0;
    }

    int                                  i;
    int                                  count;
    QMap<AbstractPhoto*, QDomElement>    data;
    QPair<PLESceneBackground*, QDomElement> background;
    QPair<PLESceneBorder*, QDomElement>     border;

    friend class PLECanvasLoadingThread;
};

PLECanvasLoadingThread::PLECanvasLoadingThread(QObject* const parent)
    : QThread(parent),
      d(new PLECanvasLoadingThreadPrivate)
{
}

PLECanvasLoadingThread::~PLECanvasLoadingThread()
{
    delete d;
}

void PLECanvasLoadingThread::progresChanged(double progress)
{
    ProgressEvent* progressUpdateEvent = new ProgressEvent(this);
    progressUpdateEvent->setData(ProgressEvent::ProgressUpdate, ((double)d->i+1)/((double)d->data.count()+1) + (progress / (double)d->data.count()+1) );
    QCoreApplication::postEvent(PLEWindow::instance(), progressUpdateEvent);
    QCoreApplication::processEvents();
}

void PLECanvasLoadingThread::progresName(const QString& name)
{
    ProgressEvent* actionUpdateEvent = new ProgressEvent(this);
    actionUpdateEvent->setData(ProgressEvent::ActionUpdate, name);
    QCoreApplication::postEvent(PLEWindow::instance(), actionUpdateEvent);
    QCoreApplication::processEvents();
}

void PLECanvasLoadingThread::addItem(AbstractPhoto* item, QDomElement& element)
{
    if (!item || element.isNull())
        return;

    d->data.insert(item, element);
}

void PLECanvasLoadingThread::addBackground(PLESceneBackground* background, QDomElement& element)
{
    if (element.attribute(QLatin1String("class")) != QLatin1String("background") || !background)
        return;

    d->background.first = background;
    d->background.second = element;
}

void PLECanvasLoadingThread::addBorder(PLESceneBorder* border, QDomElement& element)
{
    if (element.attribute(QLatin1String("class")) != QLatin1String("border") || !border)
        return;

    d->border.first = border;
    d->border.second = element;
}

void PLECanvasLoadingThread::run()
{
    ProgressEvent* startEvent = new ProgressEvent(this);
    startEvent->setData(ProgressEvent::Init, 0);
    QCoreApplication::postEvent(PLEWindow::instance(), startEvent);
    QCoreApplication::processEvents();

    // Background
    {
        ProgressEvent* actionUpdateEvent = new ProgressEvent(this);
        actionUpdateEvent->setData(ProgressEvent::ActionUpdate, QObject::tr("Loading background...") );
        QCoreApplication::postEvent(PLEWindow::instance(), actionUpdateEvent);
        QCoreApplication::processEvents();

        if (d->background.first)
        {
            PLESceneBackgroundLoader * loader = new PLESceneBackgroundLoader(d->background.first, d->background.second);
            loader->start();
            loader->wait();
        }

        ProgressEvent* progressUpdateEvent = new ProgressEvent(this);
        progressUpdateEvent->setData(ProgressEvent::ProgressUpdate, 1/((double)d->data.count()+2) );
        QCoreApplication::postEvent(PLEWindow::instance(), progressUpdateEvent);
        QCoreApplication::processEvents();
    }

    // Items
    int count = d->data.count();
    d->i = 0;

    for (QMap<AbstractPhoto*,QDomElement>::iterator it = d->data.begin(); it != d->data.end(); ++it, ++(d->i))
    {
        ProgressEvent* actionUpdateEvent = new ProgressEvent(this);
        actionUpdateEvent->setData(ProgressEvent::ActionUpdate, QObject::tr("Loading item no. %1...").arg(QString::number(d->i)));
        QCoreApplication::postEvent(PLEWindow::instance(), actionUpdateEvent);
        QCoreApplication::processEvents();

        QDomElement e = it.value();

        if      (e.attribute(QLatin1String("class")) == QLatin1String("PhotoItem"))
        {
            PhotoItemLoader * loader = new PhotoItemLoader(dynamic_cast<PhotoItem*>(it.key()), it.value());
            loader->setObserver(this);
            loader->start();
            loader->wait();
        }
        else if (e.attribute(QLatin1String("class")) == QLatin1String("TextItem"))
        {
            TextItemLoader * loader = new TextItemLoader(dynamic_cast<TextItem*>(it.key()), it.value());
            loader->setObserver(this);
            loader->start();
            loader->wait();
        }

        ProgressEvent* progressUpdateEvent = new ProgressEvent(this);
        progressUpdateEvent->setData(ProgressEvent::ProgressUpdate, ((double)d->i+1)/((double)count+2) );
        QCoreApplication::postEvent(PLEWindow::instance(), progressUpdateEvent);
        QCoreApplication::processEvents();
    }

    // Border
    {
        ProgressEvent* actionUpdateEvent = new ProgressEvent(this);
        actionUpdateEvent->setData(ProgressEvent::ActionUpdate, QObject::tr("Loading border...") );
        QCoreApplication::postEvent(PLEWindow::instance(), actionUpdateEvent);
        QCoreApplication::processEvents();

        if (d->border.first)
        {
            PLESceneBorderLoader * borderLoader = new PLESceneBorderLoader(d->border.first, d->border.second);
            borderLoader->start();
            borderLoader->wait();
        }

        ProgressEvent* progressUpdateEvent = new ProgressEvent(this);
        progressUpdateEvent->setData(ProgressEvent::ProgressUpdate, 1/((double)d->data.count()+2) );
        QCoreApplication::postEvent(PLEWindow::instance(), progressUpdateEvent);
        QCoreApplication::processEvents();
    }

    ProgressEvent* finishEvent = new ProgressEvent(this);
    finishEvent->setData(ProgressEvent::Finish, 0);
    QCoreApplication::postEvent(PLEWindow::instance(), finishEvent);
    QCoreApplication::processEvents();
}

} // namespace PhotoLayoutsEditor
