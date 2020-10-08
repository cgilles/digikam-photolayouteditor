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

#ifndef ABSTRACT_ITEMS_LIST_VIEW_TOOL_H
#define ABSTRACT_ITEMS_LIST_VIEW_TOOL_H

// Local includes

#include "abstractitemstool.h"
#include "abstractmovablemodel.h"

namespace PhotoLayoutsEditor
{

class AbstractItemsListViewToolPrivate;
class AbstractListToolView;
class AbstractListToolViewDelegate;

class AbstractItemsListViewTool : public AbstractItemsTool
{
    Q_OBJECT

public:

    explicit AbstractItemsListViewTool(const QString& toolsName,
                                       PLEScene* scene,
                                       PLECanvas::SelectionMode selectionMode,
                                       QWidget* parent=nullptr);
    ~AbstractItemsListViewTool() override;

    void currentItemAboutToBeChanged() override;
    void currentItemChanged() override;
    void positionAboutToBeChanged() override
    {
    } // Unused

    void positionChanged() override
    {
    }          // Unused

    virtual QStringList options() const = 0;
    virtual QObject* createItem(const QString& name) = 0;

protected Q_SLOTS:

    void viewCurrentEditor(const QModelIndex& index);
    void viewCurrentEditor(QObject* object);
    void createChooser();
    void closeChooser();
    void removeSelected();
    void moveSelectedDown();
    void moveSelectedUp();

    virtual AbstractMovableModel* model() = 0;
    virtual QWidget* createEditor(QObject* item, bool createCommands=true) = 0;

private:

    void closeEditor();

private:

    AbstractItemsListViewToolPrivate* d;
};

} // namespace PhotoLayoutsEditor

#endif // ABSTRACT_ITEMS_LIST_VIEW_TOOL_H
