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

#include "PLEConfigViewWidget.h"

// Qt includes

#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>

// Local incudes

#include "PLEConfigSkeleton.h"

using namespace PhotoLayoutsEditor;

class PhotoLayoutsEditor::PLEConfigViewWidget::PLEConfigViewWidgetPrivate
{
    QCheckBox*      antialiasing;
    QDoubleSpinBox* xGrid;
    QDoubleSpinBox* yGrid;
    QCheckBox*      showGrid;

    friend class PLEConfigViewWidget;
};

PLEConfigViewWidget::PLEConfigViewWidget(QWidget* const parent, const QString& caption)
  : QWidget(parent),
    d(new PLEConfigViewWidgetPrivate)
{
    this->setupGUI();
    this->setWindowTitle(caption);
}

PLEConfigViewWidget::~PLEConfigViewWidget()
{
    delete d;
}

void PLEConfigViewWidget::updateSettings()
{
    PLEConfigSkeleton::setAntialiasing(d->antialiasing->isChecked());
    PLEConfigSkeleton::setShowGrid(d->showGrid->isChecked());
    PLEConfigSkeleton::setHorizontalGrid(d->xGrid->value());
    PLEConfigSkeleton::setVerticalGrid(d->yGrid->value());
    PLEConfigSkeleton::self()->save();
}

void PLEConfigViewWidget::updateWidgets()
{
    d->antialiasing->setChecked(PLEConfigSkeleton::antialiasing());
    d->showGrid->setChecked(PLEConfigSkeleton::showGrid());
    d->xGrid->setValue(PLEConfigSkeleton::horizontalGrid());
    d->yGrid->setValue(PLEConfigSkeleton::verticalGrid());
}

void PLEConfigViewWidget::setupGUI()
{
    QVBoxLayout* const layout = new QVBoxLayout();
    this->setLayout(layout);

    PLEConfigSkeleton* skeleton = PLEConfigSkeleton::self();

    QFormLayout* const generalLayout = new QFormLayout();
    layout->addLayout(generalLayout);
    d->antialiasing = new QCheckBox(this);
    connect(skeleton, SIGNAL(antialiasingChanged(bool)), d->antialiasing, SLOT(setChecked(bool)));
    generalLayout->addRow(QObject::tr("Antialiasing"), d->antialiasing);

    QGroupBox* const gridBox = new QGroupBox(QObject::tr("Grid"), this);
    layout->addWidget(gridBox);
    QFormLayout* const gridLayout = new QFormLayout();
    gridBox->setLayout(gridLayout);

    d->showGrid = new QCheckBox(gridBox);
    connect(skeleton, SIGNAL(showGridChanged(bool)), d->showGrid, SLOT(setChecked(bool)));
    gridLayout->addRow(QObject::tr("Show grid lines"), d->showGrid);

    d->xGrid = new QDoubleSpinBox(gridBox);
    KConfigSkeletonItem* hgi = skeleton->findItem(QLatin1String("horizontalGrid"));
    if (hgi)
    {
        d->xGrid->setMinimum(hgi->minValue().toDouble());
        d->xGrid->setMaximum(hgi->maxValue().toDouble());
    }
    d->xGrid->setSingleStep(1.0);
    connect(skeleton, SIGNAL(horizontalGridChanged(double)), d->xGrid, SLOT(setValue(double)));
    gridLayout->addRow(QObject::tr("Horizontal distance"), d->xGrid);

    d->yGrid = new QDoubleSpinBox(gridBox);
    KConfigSkeletonItem* vgi = skeleton->findItem(QLatin1String("verticalGrid"));

    if (hgi)
    {
        d->yGrid->setMinimum(vgi->minValue().toDouble());
        d->yGrid->setMaximum(vgi->maxValue().toDouble());
    }

    d->yGrid->setSingleStep(1.0);
    connect(skeleton, SIGNAL(verticalGridChanged(double)), d->yGrid, SLOT(setValue(double)));
    gridLayout->addRow(QObject::tr("Vertical distance"), d->yGrid);

    this->updateWidgets();
}

