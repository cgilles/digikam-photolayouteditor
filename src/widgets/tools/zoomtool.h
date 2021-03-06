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

#ifndef ZOOM_TOOL_H
#define ZOOM_TOOL_H

// Local inludes

#include "abstracttool.h"

namespace PhotoLayoutsEditor
{

class ZoomTool : public AbstractTool
{
    Q_OBJECT

public:

    explicit ZoomTool(PLEScene* scene, QWidget* parent = nullptr);
    ~ZoomTool() override;

public Q_SLOTS:

    void zoom(const QPointF& point);

protected:

    void sceneChange()  override;
    void sceneChanged() override;

private:

    class ZoomToolPrivate;
    ZoomToolPrivate* d;
    friend class ZoomToolPrivate;
};

} // namespace PhotoLayoutsEditor

#endif // ZOOM_TOOL_H
