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

// Qt Includes

#include <QString>
#include <QApplication>
#include <QIcon>
#include <QCommandLineParser>
#include <QCommandLineOption>

// digiKam includes

#include <metaengine.h>

// Local includes

#include "plewindow.h"

using namespace PhotoLayoutsEditor;
using namespace Digikam;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QLatin1String(":/photolayoutseditor.png")));

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addPositionalArgument(QLatin1String("file"), QObject::tr("Template file to open"), QLatin1String("+[file]"));
    parser.process(app);

    MetaEngine::initializeExiv2();

    QList<QUrl> urlList;
    const QStringList args = parser.positionalArguments();

    for (auto& arg : args)
    {
        urlList.append(QUrl::fromLocalFile(arg));
    }

    parser.clearPositionalArguments();

    PLEWindow* const w = PLEWindow::instance(nullptr);
    w->openFile();

    if (!urlList.isEmpty())
    {
        w->loadImages(urlList);
    }

    w->show();

    int result = app.exec();
    MetaEngine::cleanupExiv2();

    return result;
}
