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

#include "plecanvassavingthread.h"

// Qt includes

#include <QFile>
#include <QDomDocument>
#include <QCoreApplication>
#include <QDebug>

// Local includes

#include "plecanvas_p.h"
#include "plescene.h"
#include "progressevent.h"
#include "plewindow.h"

namespace PhotoLayoutsEditor
{

PLECanvasSavingThread::PLECanvasSavingThread(QObject* parent)
    : QThread(parent),
      m_canvas(nullptr),
      m_template(false)
{
}

void PLECanvasSavingThread::save(PLECanvas* canvas, const QUrl& url)
{
    m_canvas = canvas;
    m_url    = url;
    this->start();
}

void PLECanvasSavingThread::saveAsTemplate(PLECanvas* canvas, const QUrl& url)
{
    m_canvas   = canvas;
    m_url      = url;
    m_template = true;
    this->start();
}

void PLECanvasSavingThread::progresChanged(double progress)
{
    this->sendProgressUpdate(0.05 + progress * 0.75);
}

void PLECanvasSavingThread::progresName(const QString& name)
{
    this->sendActionUpdate(name);
}

void PLECanvasSavingThread::run()
{
    if (!m_canvas || !m_url.isValid())
        return;

    //---------------------------------------------------------------------------

    ProgressEvent* startEvent = new ProgressEvent(this);
    startEvent->setData(ProgressEvent::Init, 0);
    QCoreApplication::postEvent(PLEWindow::instance(), startEvent);
    QCoreApplication::processEvents();

    this->sendActionUpdate( QObject::tr("Creating canvas...") );

    QRect sceneRect = m_canvas->sceneRect().toRect();
    QDomDocument document(QLatin1String(" svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\""));
    QDomElement svg = document.createElement(QLatin1String("svg"));
    document.appendChild(svg);

    svg.setAttribute(QLatin1String("xmlns"), QLatin1String("http://www.w3.org/2000/svg"));
    svg.setAttribute(QLatin1String("viewBox"), QString::number(sceneRect.x()) + QLatin1Char(' ') +
                                QString::number(sceneRect.y()) + QLatin1Char(' ') +
                                QString::number(sceneRect.width()) + QLatin1Char(' ') +
                                QString::number(sceneRect.height()));
    svg.setAttribute(QLatin1String("xmlns:xlink"), QLatin1String("http://www.w3.org/1999/xlink"));
    svg.setAttribute(QLatin1String("version"), QLatin1String("1.2"));
    svg.setAttribute(QLatin1String("baseProfile"), QLatin1String("tiny"));
    QString j1;
    
    switch (m_canvas->d->m_size.sizeUnit())
    {
        case PLECanvasSize::Centimeters:
            j1 = QLatin1String("cm");
            break;

        case PLECanvasSize::Milimeters:
            j1 = QLatin1String("mm");
            break;

        case PLECanvasSize::Inches:
            j1 = QLatin1String("in");
            break;

        case PLECanvasSize::Picas:
            j1 = QLatin1String("pc");
            break;

        case PLECanvasSize::Points:
            j1 = QLatin1String("pt");
            break;

        case PLECanvasSize::Pixels:
            j1 = QLatin1String("px");
            break;

        default:
            j1 = QLatin1String("px");
            qDebug() << "Unhandled size unit at:" << __FILE__ << ":" << __LINE__;
            break;
    }

    svg.setAttribute(QLatin1String("width"), QString::number(m_canvas->d->m_size.size().width()) + j1);
    svg.setAttribute(QLatin1String("height"), QString::number(m_canvas->d->m_size.size().height()) + j1);
    QDomElement resolution = document.createElementNS(m_template ? PhotoLayoutsEditor::templateUri() : PhotoLayoutsEditor::uri(), QLatin1String("page"));
    resolution.setAttribute(QLatin1String("width"), QString::number(m_canvas->d->m_size.resolution().width()));
    resolution.setAttribute(QLatin1String("height"), QString::number(m_canvas->d->m_size.resolution().height()));
    resolution.setAttribute(QLatin1String("unit"), PLECanvasSize::resolutionUnitName(m_canvas->d->m_size.resolutionUnit()));
    svg.appendChild(resolution);

    this->sendProgressUpdate( 0.05 );
    this->sendActionUpdate( QObject::tr("Saving scene...") );

    //---------------------------------------------------------------------------

    PLEScene* const scene = dynamic_cast<PLEScene*>(m_canvas->scene());

    if (!scene)
    {
        this->exit(1);
        return;
    }

    QDomDocument sceneDocument = m_template ? scene->toTemplateSvg(this) : scene->toSvg(this);
    QDomElement sceneElement = sceneDocument.documentElement();

    if (sceneElement.isNull())
        this->exit(1);

    svg.appendChild(sceneElement);

    //---------------------------------------------------------------------------

    this->sendProgressUpdate( 0.8 );
    this->sendActionUpdate( QObject::tr("Encoding data...") );

    QFile file(m_url.path());

    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QByteArray result = document.toByteArray();
        const char * data = result.data();
        int i = 0;
        const int limit = result.size();
        int j2 = 1000;
        j2 = (j2 > limit ? limit : j2);
        this->sendActionUpdate( QObject::tr("Writing data to file...") );

        while (i < limit)
        {
            i += file.write(data+i, (i+j2 <= limit ? j2 : limit-i));
            this->sendProgressUpdate( 0.8 + 0.2 * ((double)i / (double)limit) );
        }

        file.close();
        emit saved();
    }

    //---------------------------------------------------------------------------

    ProgressEvent* const finishEvent = new ProgressEvent(this);
    finishEvent->setData(ProgressEvent::Finish, 0);
    QCoreApplication::postEvent(PLEWindow::instance(), finishEvent);
    QCoreApplication::processEvents();

    this->exit(0);
}

void PLECanvasSavingThread::bytesWritten(qint64 b)
{
    qDebug() << "writen" << b;
}

void PLECanvasSavingThread::sendProgressUpdate(double v)
{
    ProgressEvent* event = new ProgressEvent(this);
    event->setData(ProgressEvent::ProgressUpdate, v);
    QCoreApplication::postEvent(PLEWindow::instance(), event);
    QCoreApplication::processEvents();
}

void PLECanvasSavingThread::sendActionUpdate(const QString& str)
{
    ProgressEvent* event = new ProgressEvent(this);
    event->setData(ProgressEvent::ActionUpdate, str);
    QCoreApplication::postEvent(PLEWindow::instance(), event);
    QCoreApplication::processEvents();
}

} // namespace PhotoLayoutsEditor
