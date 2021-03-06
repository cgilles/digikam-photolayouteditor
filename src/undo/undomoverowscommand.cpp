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

#include "undomoverowscommand.h"

// Qt includes

#include <QDebug>

// Local incudes

#include "layersmodel.h"
#include "layersmodelitem.h"

namespace PhotoLayoutsEditor
{

UndoMoveRowsCommand::UndoMoveRowsCommand(int startingRow,
                                         int rowsCount,
                                         const QModelIndex& sourceParent,
                                         int destinationRow,
                                         const QModelIndex& destinationParent,
                                         LayersModel* model,
                                         QUndoCommand* parent)
    : QUndoCommand(parent),
      m_model(model)
{
    if (m_model)
    {
        if (sourceParent == destinationParent)
        {
            if (startingRow > destinationRow)
                this->setText(QObject::tr("Move layers up"));
            else
                this->setText(QObject::tr("Move layers down"));
        }
        else
        {
            this->setText(QObject::tr("Change parent layer"));
        }

        m_src_parent_row = model->getItem(sourceParent);
        m_dest_parent_row = model->getItem(destinationParent);
        m_starting_row = startingRow;
        m_rows_count = rowsCount;
        m_destination_row = destinationRow;
    }
    else
    {
        m_src_parent_row = nullptr;
        m_dest_parent_row = nullptr;
        m_starting_row = 0;
        m_rows_count = 0;
        m_destination_row = 0;

        qDebug() << "Can't create UndoMoveRowsCommand [NO MODEL!]:";
        qDebug() << "\tStarting Row =" << startingRow;
        qDebug() << "\tRows count =" << rowsCount;
        qDebug() << "\tDestination Row =" << destinationRow;
        qDebug() << "\tSource Parent =" << sourceParent;
        qDebug() << "\tDestination Parent =" << destinationParent;
    }
}

void UndoMoveRowsCommand::redo()
{
    if (m_model &&
        m_model->moveRows( m_starting_row, m_rows_count, m_model->findIndex(m_src_parent_row), m_destination_row, m_model->findIndex(m_dest_parent_row) ))
    {
        reverse();
    }
    else
    {

#ifdef QT_DEBUG

        qDebug() << "Can't redo from UndoMoveRowsCommand:";
        qDebug() << "\tStarting Row =" << m_starting_row;
        qDebug() << "\tRows count =" << m_rows_count;
        qDebug() << "\tDestination Row =" << m_destination_row;

        if (m_model)
        {
            qDebug() << "\tSource Parent =" << m_model->findIndex(m_src_parent_row);
            qDebug() << "\tDestination Parent =" << m_model->findIndex(m_dest_parent_row);
        }

#endif

    }
}

void UndoMoveRowsCommand::undo()
{
    if (m_model &&
        m_model->moveRows( m_starting_row, m_rows_count, m_model->findIndex(m_src_parent_row), m_destination_row, m_model->findIndex(m_dest_parent_row) ))
    {
        reverse();
    }
    else
    {

#ifdef QT_DEBUG

        qDebug() << "Can't undo from UndoMoveRowsCommand:";
        qDebug() << "\tStarting Row =" << m_starting_row;
        qDebug() << "\tRows count =" << m_rows_count;
        qDebug() << "\tDestination Row =" << m_destination_row;

        if (m_model)
        {
            qDebug() << "\tSource Parent =" << m_model->findIndex(m_src_parent_row);
            qDebug() << "\tDestination Parent =" << m_model->findIndex(m_dest_parent_row);
        }

#endif

    }
}

void UndoMoveRowsCommand::reverse()
{
    int temp = m_destination_row;
    m_destination_row = m_starting_row;
    m_starting_row = temp;

    if (m_dest_parent_row == m_src_parent_row)
    {
        if (m_destination_row > m_starting_row)
            m_destination_row += m_rows_count;
        else
            m_starting_row -= m_rows_count;
    }
    else
    {
        LayersModelItem* temp2 = m_dest_parent_row;
        m_dest_parent_row = m_src_parent_row;
        m_src_parent_row = temp2;
    }
}

} // namespace PhotoLayoutsEditor
