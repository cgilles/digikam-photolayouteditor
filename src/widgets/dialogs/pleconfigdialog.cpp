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

#include "pleconfigdialog.h"

// Qt includes

#include <QSettings>
#include <QIcon>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>

namespace PhotoLayoutsEditor
{

class PLEConfigDialog::Private
{
public:

    explicit Private()
        : antialiasing(nullptr),
          xGrid(nullptr),
          yGrid(nullptr),
          showGrid(nullptr)
    {
    }

    QCheckBox*      antialiasing;
    QDoubleSpinBox* xGrid;
    QDoubleSpinBox* yGrid;
    QCheckBox*      showGrid;
};

PLEConfigDialog::PLEConfigDialog(QWidget* const parent)
    : QDialog(parent),
      d(new Private)
{
    QVBoxLayout* const vlay          = new QVBoxLayout();
    setLayout(vlay);

    d->antialiasing                  = new QCheckBox(QObject::tr("Antialiasing"), this);
    vlay->addWidget(d->antialiasing);

    QGroupBox* const gridBox         = new QGroupBox(QObject::tr("Grid"), this);
    QFormLayout* const gridLayout    = new QFormLayout();
    gridBox->setLayout(gridLayout);

    d->showGrid                      = new QCheckBox(gridBox);

    gridLayout->addRow(QObject::tr("Show grid lines"), d->showGrid);

    d->xGrid                         = new QDoubleSpinBox(gridBox);
    d->xGrid->setMinimum(2.0);
    d->xGrid->setMaximum(50.0);
    d->xGrid->setSingleStep(1.0);

    gridLayout->addRow(QObject::tr("Horizontal distance"), d->xGrid);

    d->yGrid                         = new QDoubleSpinBox(gridBox);
    d->yGrid->setMinimum(2.0);
    d->yGrid->setMaximum(50.0);
    d->yGrid->setSingleStep(1.0);

    gridLayout->addRow(QObject::tr("Vertical distance"), d->yGrid);

    vlay->addWidget(gridBox);

    // ---

    QDialogButtonBox* const buttons  = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Ok)->setDefault(true);
    vlay->addWidget(buttons);

    connect(buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this, SLOT(accept()));

    connect(buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            this, SLOT(reject()));

    adjustSize();

    loadSettings();
}

PLEConfigDialog::~PLEConfigDialog()
{
    saveSettings();
    delete d;
}

void PLEConfigDialog::saveSettings()
{
    QSettings config(QLatin1String("PhotoLayoutEditor"));
    config.beginGroup(QLatin1String("View"));

    config.setValue(QLatin1String("Antialiasing"), d->antialiasing->isChecked());
    config.setValue(QLatin1String("ShowGrid"),     d->showGrid->isChecked());
    config.setValue(QLatin1String("XGrid"),        d->xGrid->value());
    config.setValue(QLatin1String("YGrid"),        d->yGrid->value());

    config.endGroup();
    config.sync();
}

void PLEConfigDialog::loadSettings()
{
    QSettings config(QLatin1String("PhotoLayoutEditor"));
    config.beginGroup(QLatin1String("View"));

    d->antialiasing->setChecked(config.value(QLatin1String("Antialiasing"), false).toBool());
    d->showGrid->setChecked(config.value(QLatin1String("ShowGrid"), false).toBool());
    d->xGrid->setValue(config.value(QLatin1String("XGrid"), 25.0).toDouble());
    d->yGrid->setValue(config.value(QLatin1String("YGrid"), 25.0).toDouble());

    config.endGroup();
}

} // namespace PhotoLayoutsEditor
