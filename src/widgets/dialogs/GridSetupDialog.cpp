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

#include "GridSetupDialog.h"

// Qt includes

#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSettings>

namespace PhotoLayoutsEditor
{

GridSetupDialog::GridSetupDialog(QWidget* const parent)
    : QDialog(parent)
{
    setWindowTitle(QObject::tr("Setup grid lines"));
    setModal(true);

    centralWidget             = new QWidget(this);
    QFormLayout* const layout = new QFormLayout(centralWidget);
    layout->setSizeConstraint( QLayout::SetFixedSize );

    setLayout(layout);

    x = new QDoubleSpinBox(centralWidget);
    x->setMinimum(2.0);
    x->setMaximum(50.0);
    x->setSingleStep(1.0);

    layout->addRow(QObject::tr("Horizontal distance"), x);

    y = new QDoubleSpinBox(centralWidget);
    y->setMinimum(2.0);
    y->setMaximum(50.0);
    y->setSingleStep(1.0);

    layout->addRow(QObject::tr("Vertical distance"), y);

    QDialogButtonBox* const buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Ok)->setDefault(true);
    layout->addRow(buttons);

    connect(buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this, SLOT(accept()));

    connect(buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            this, SLOT(reject()));
    
    QSettings config;
    config.beginGroup(QLatin1String("View"));

    setHorizontalDistance(config.value(QLatin1String("XGrid"), 25.0).toDouble());
    setVerticalDistance(config.value(QLatin1String("YGrid"), 25.0).toDouble());
}

void GridSetupDialog::setHorizontalDistance(qreal value)
{
    x->setValue(value);
}

void GridSetupDialog::setVerticalDistance(qreal value)
{
    y->setValue(value);
}

qreal GridSetupDialog::horizontalDistance() const
{
    return x->value();
}

qreal GridSetupDialog::verticalDistance() const
{
    return y->value();
}

int GridSetupDialog::exec()
{
    int result = QDialog::exec();

    if (result == Accepted)
    {
        QSettings config;
        config.beginGroup(QLatin1String("View"));
        
        config.setValue(QLatin1String("XGrid"), horizontalDistance());
        config.setValue(QLatin1String("YGrid"), verticalDistance());

        config.endGroup();
        config.sync();
    }

    return result;
}

} // namespace PhotoLayoutsEditor
