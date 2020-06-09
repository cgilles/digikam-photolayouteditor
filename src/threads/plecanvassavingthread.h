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

#ifndef CANVASSAVINGTHREAD_H
#define CANVASSAVINGTHREAD_H

#include <QThread>
#include <QUrl>

#include "progressobserver.h"

namespace PhotoLayoutsEditor
{
    class PLECanvas;
    class PLECanvasSavingThread : public QThread, public ProgressObserver
    {
            Q_OBJECT

        public:

            explicit PLECanvasSavingThread(QObject* parent = nullptr);
            void save(PLECanvas* canvas, const QUrl& url);
            void saveAsTemplate(PLECanvas* canvas, const QUrl& url);
            virtual void progresChanged(double progress) override;
            virtual void progresName(const QString& name) override;

        Q_SIGNALS:

            void saved();

        protected:

            virtual void run() override;

        private Q_SLOTS:

            void bytesWritten(qint64);

        private:

            void sendProgressUpdate(double v);
            void sendActionUpdate(const QString& str);

        private:

            PLECanvas* m_canvas;
            QUrl    m_url;
            bool    m_template;
    };
}

#endif // CANVASSAVINGTHREAD_H
