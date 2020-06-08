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

#ifndef ABSTRACTTOOL_H
#define ABSTRACTTOOL_H

// Qt includes

#include <QWidget>

// Local includes

#include "plescene.h"
#include "plecanvas.h"
#include "toolsdockwidget.h"

namespace PhotoLayoutsEditor
{
    class ToolsDockWidget;

    class AbstractTool : public QWidget
    {
            Q_OBJECT

            PLEScene * m_scene;

            PLECanvas::SelectionMode sel_mode;

        public:

            AbstractTool(PLEScene * scene, PLECanvas::SelectionMode selectionMode, QWidget* parent = nullptr) :
                QWidget(parent),
                m_scene(scene),
                sel_mode(selectionMode)
            {}

            PLEScene * scene() const
            {
                return m_scene;
            }

        protected:

            void setScene(PLEScene * scene)
            {
                if (m_scene == scene)
                    return;
                this->sceneChange();
                this->m_scene = scene;
                if (scene)
                {
                    connect(m_scene, SIGNAL(destroyed()), this, SLOT(sceneDestroyed()));
                    this->setEnabled(true);
                }
                else
                    this->setEnabled(false);
                this->sceneChanged();
            }

            virtual void sceneChange()
            {}

            virtual void sceneChanged()
            {}

        protected Q_SLOTS:

            void sceneDestroyed()
            {
                if (sender() == m_scene)
                    this->setScene(nullptr);
            }

        friend class ToolsDockWidget;
    };
}

#endif // ABSTRACTTOOL_H
