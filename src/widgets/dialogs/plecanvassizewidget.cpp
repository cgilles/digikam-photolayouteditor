/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2011-09-01
 * Description : a plugin to create photo layouts by fusion of several images.
 *
 * Copyright (C) 2011-2012 by Lukasz Spas <lukasz dot spas at gmail dot com>
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

#include "plecanvassizewidget.h"

// Qt includes

#include <QComboBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

// Local includes

#include "plecanvassize.h"

namespace PhotoLayoutsEditor
{

class PLECanvasSizeWidget::Private
{
    explicit Private()
        : sizeWidget(nullptr),
          advancedWidget(nullptr),
          xSize(nullptr),
          ySize(nullptr),
          sizeLabel(nullptr),
          sizeUnitsWidget(nullptr),
          xResolution(nullptr),
          yResolution(nullptr),
          resolutionUnitsWidget(nullptr)
    {
    }

    void swapSizes();
    void updateSizeLabel();

    QWidget*        sizeWidget;
    QWidget*        advancedWidget;
    QDoubleSpinBox * xSize;
    QDoubleSpinBox * ySize;
    QLabel *         sizeLabel;
    QComboBox *      sizeUnitsWidget;
    QDoubleSpinBox * xResolution;
    QDoubleSpinBox * yResolution;
    QComboBox *      resolutionUnitsWidget;

    static int WIDTH;
    static int HEIGHT;
    static QString currentSizeUnit;

    static qreal WIDTH_RES;
    static qreal HEIGHT_RES;
    static QString currentResolutionUnit;

    friend class PLECanvasSizeWidget;
};

int PLECanvasSizeWidget::Private::WIDTH = 800;
int PLECanvasSizeWidget::Private::HEIGHT = 800;
QString PLECanvasSizeWidget::Private::currentSizeUnit;
qreal PLECanvasSizeWidget::Private::WIDTH_RES = 72;
qreal PLECanvasSizeWidget::Private::HEIGHT_RES = 72;
QString PLECanvasSizeWidget::Private::currentResolutionUnit;

void PLECanvasSizeWidget::Private::swapSizes()
{
    // swap dimensions
    qreal temp = WIDTH;
    WIDTH = HEIGHT;
    HEIGHT = temp;
    temp = xSize->value();
    xSize->setValue( ySize->value() );
    ySize->setValue( temp );

    // Swap resolutions
    temp = WIDTH_RES;
    WIDTH_RES = HEIGHT_RES;
    HEIGHT_RES = temp;
    temp = xResolution->value();
    xResolution->setValue(yResolution->value());
    yResolution->setValue(temp);
}

void PLECanvasSizeWidget::Private::updateSizeLabel()
{
    sizeLabel->setText(QString::number(WIDTH)
                        .append(QLatin1String(" x "))
                        .append(QString::number(HEIGHT)
                        .append(QLatin1String(" px"))));
}

PLECanvasSizeWidget::PLECanvasSizeWidget(QWidget *parent) :
    QWidget(parent),
    d(new Private)
{
    setupUI(QSize(d->WIDTH, d->HEIGHT),
            d->currentSizeUnit,
            QSize(d->WIDTH_RES, d->HEIGHT_RES),
            d->currentResolutionUnit);
}

PLECanvasSizeWidget::Orientation PLECanvasSizeWidget::orientation() const
{
    if (d->WIDTH < d->HEIGHT)
        return PLECanvasSizeWidget::Vertical;
    else
        return PLECanvasSizeWidget::Horizontal;
}

void PLECanvasSizeWidget::setupUI(const QSizeF & size, const QString& sizeUnits, const QSizeF & resolution, const QString& resolutionUnits)
{
    QString tempSizeUnits = sizeUnits;
    QString tempResolutionUnits = resolutionUnits;
    if (tempSizeUnits.isEmpty() ||
            PLECanvasSize::sizeUnit(tempSizeUnits) == PLECanvasSize::UnknownSizeUnit)
        tempSizeUnits = PLECanvasSize::sizeUnitName(PLECanvasSize::Pixels);;
    if (tempResolutionUnits.isEmpty() ||
            PLECanvasSize::resolutionUnit(tempResolutionUnits) == PLECanvasSize::UnknownResolutionUnit)
        tempResolutionUnits = PLECanvasSize::resolutionUnitName(PLECanvasSize::PixelsPerInch);;

    QVBoxLayout* vLayout = new  QVBoxLayout();
    this->setLayout(vLayout);

    /// ----------------------- CANVAS SIZE PART -----------------------
    d->sizeWidget = new QGroupBox(QObject::tr("PLECanvas size"), this);
    vLayout->addWidget(d->sizeWidget);

    QGridLayout * gridLayout = new QGridLayout(d->sizeWidget);
    d->sizeWidget->setLayout(gridLayout);

    // Width widget
    d->xSize = new QDoubleSpinBox(d->sizeWidget);
    d->xSize->setMinimum(0.00001);
    d->xSize->setMaximum(999999);
    d->xSize->setValue(size.width());
    d->WIDTH = PLECanvasSize::toPixels(size.width(),
                                    resolution.width(),
                                    PLECanvasSize::sizeUnit(tempSizeUnits),
                                    PLECanvasSize::resolutionUnit(tempResolutionUnits));
    gridLayout->addWidget(new QLabel(QObject::tr("Width"), d->sizeWidget),0,0);
    gridLayout->addWidget(d->xSize,0,1);

    // Height widget
    d->ySize = new QDoubleSpinBox(d->sizeWidget);
    d->ySize->setMinimum(0.00001);
    d->ySize->setMaximum(999999);
    d->ySize->setValue(size.height());
    d->HEIGHT = PLECanvasSize::toPixels(size.height(),
                                     resolution.height(),
                                     PLECanvasSize::sizeUnit(tempSizeUnits),
                                     PLECanvasSize::resolutionUnit(tempResolutionUnits));
    gridLayout->addWidget(new QLabel(QObject::tr("Height"), d->sizeWidget),1,0);
    gridLayout->addWidget(d->ySize,1,1);

    // Unit widget
    d->sizeUnitsWidget = new QComboBox(d->sizeWidget);
    d->sizeUnitsWidget->addItems(PLECanvasSize::sizeUnitsNames());
    d->sizeUnitsWidget->setCurrentText(tempSizeUnits);
    d->currentSizeUnit = tempSizeUnits;
    gridLayout->addWidget(d->sizeUnitsWidget,1,2);

    // Size label
    d->sizeLabel = new QLabel(d->sizeWidget);
    gridLayout->addWidget(d->sizeLabel,2, 2);

    /// ----------------------- ADVANCED PART -----------------------
    d->advancedWidget = new QGroupBox(QObject::tr("Advanced"), this);
    vLayout->addWidget(d->advancedWidget);
    gridLayout = new QGridLayout(d->advancedWidget);
    d->advancedWidget->setLayout(gridLayout);

    // x resolution widget
    d->xResolution = new QDoubleSpinBox(d->advancedWidget);
    d->xResolution->setMinimum(0);
    d->xResolution->setMaximum(999999);
    d->xResolution->setValue(resolution.width());
    d->xResolution->setDecimals(3);
    d->WIDTH_RES = resolution.width() * PLECanvasSize::resolutionUnitFactor(tempResolutionUnits);
    gridLayout->addWidget(new QLabel(QObject::tr("Resolution X"), d->advancedWidget),0,0);
    gridLayout->addWidget(d->xResolution,0,1);

    // y resolution widget
    d->yResolution = new QDoubleSpinBox(d->advancedWidget);
    d->yResolution->setMinimum(0);
    d->yResolution->setMaximum(999999);
    d->yResolution->setValue(resolution.height());
    d->yResolution->setDecimals(3);
    d->HEIGHT_RES = resolution.height() * PLECanvasSize::resolutionUnitFactor(tempResolutionUnits);
    gridLayout->addWidget(new QLabel(QObject::tr("Resolution Y"), d->advancedWidget),1,0);
    gridLayout->addWidget(d->yResolution,1,1);

    // Unit widget
    d->resolutionUnitsWidget = new QComboBox(d->sizeWidget);
    d->resolutionUnitsWidget->addItems(PLECanvasSize::resolutionUnitsNames());
    d->resolutionUnitsWidget->setCurrentText(tempResolutionUnits);
    d->currentResolutionUnit = tempResolutionUnits;
    gridLayout->addWidget(d->resolutionUnitsWidget,1,2);

    this->prepareSignalsConnections();

    d->updateSizeLabel();
}

PLECanvasSizeWidget::~PLECanvasSizeWidget()
{
    delete d;
}

void PLECanvasSizeWidget::prepareSignalsConnections()
{
    connect(d->xSize, SIGNAL(valueChanged(double)), this, SLOT(widthChanged(double)));
    connect(d->ySize, SIGNAL(valueChanged(double)), this, SLOT(heightChanged(double)));
    connect(d->sizeUnitsWidget, SIGNAL(activated(QString)), this, SLOT(sizeUnitsChanged(QString)));
    connect(d->xResolution, SIGNAL(valueChanged(double)), this, SLOT(xResolutionChanged(double)));
    connect(d->yResolution, SIGNAL(valueChanged(double)), this, SLOT(yResolutionChanged(double)));
    connect(d->resolutionUnitsWidget, SIGNAL(currentIndexChanged(QString)), this, SLOT(resolutionUnitsChanged(QString)));
}

PLECanvasSize PLECanvasSizeWidget::canvasSize() const
{
    PLECanvasSize result(QSizeF(d->xSize->value(), d->ySize->value()),
                      PLECanvasSize::sizeUnit(d->sizeUnitsWidget->currentText()),
                      QSizeF(d->xResolution->value(), d->yResolution->value()),
                      PLECanvasSize::resolutionUnit(d->resolutionUnitsWidget->currentText()));
    return result;
}

void PLECanvasSizeWidget::sizeUnitsChanged(const QString& unitName)
{
    d->currentSizeUnit = unitName;
    PLECanvasSize::SizeUnits sizeUnit = PLECanvasSize::sizeUnit(unitName);
    if (sizeUnit == PLECanvasSize::Pixels)
    {
        d->xSize->setValue(d->WIDTH);
        d->ySize->setValue(d->HEIGHT);
        d->xSize->setDecimals(0);
        d->ySize->setDecimals(0);
        return;
    }
    d->xSize->setDecimals(5);
    d->ySize->setDecimals(5);
    PLECanvasSize::ResolutionUnits resolutionUnit = PLECanvasSize::resolutionUnit(d->resolutionUnitsWidget->currentText());
    qreal WIDTH = PLECanvasSize::fromPixels(d->WIDTH,
                                         d->xResolution->value(),
                                         sizeUnit,
                                         resolutionUnit);
    qreal HEIGHT = PLECanvasSize::fromPixels(d->HEIGHT,
                                          d->yResolution->value(),
                                          sizeUnit,
                                          resolutionUnit);
    d->xSize->setValue(WIDTH);
    d->ySize->setValue(HEIGHT);
}

void PLECanvasSizeWidget::resolutionUnitsChanged(const QString& unitName)
{
    d->currentResolutionUnit = unitName;
    PLECanvasSize::ResolutionUnits unit = PLECanvasSize::resolutionUnit(unitName);
    if (unit == PLECanvasSize::PixelsPerInch)
    {
        d->xResolution->setValue(d->WIDTH_RES);
        d->yResolution->setValue(d->HEIGHT_RES);
        return;
    }
    qreal factor = PLECanvasSize::resolutionUnitFactor(unit);
    d->xResolution->setValue(d->WIDTH_RES / factor);
    d->yResolution->setValue(d->HEIGHT_RES / factor);
}

void PLECanvasSizeWidget::setHorizontal(bool isHorizontal)
{
    if (isHorizontal)
    {
        if (d->WIDTH < d->HEIGHT)
        {
            d->swapSizes();
            d->updateSizeLabel();
        }
        emit orientationChanged();
    }
}

void PLECanvasSizeWidget::setVertical(bool isVertical)
{
    if (isVertical)
    {
        if (d->HEIGHT < d->WIDTH)
        {
            d->swapSizes();
            d->updateSizeLabel();
        }
        emit orientationChanged();
    }
}

void PLECanvasSizeWidget::widthChanged(double width)
{
    width = PLECanvasSize::toPixels(width,
                                 d->xResolution->value(),
                                 PLECanvasSize::sizeUnit(d->sizeUnitsWidget->currentText()),
                                 PLECanvasSize::resolutionUnit(d->resolutionUnitsWidget->currentText()));
    d->WIDTH = width;
    this->setHorizontal(d->WIDTH > d->HEIGHT);
    this->setVertical(d->WIDTH < d->HEIGHT);
    d->updateSizeLabel();
}

void PLECanvasSizeWidget::heightChanged(double height)
{
    height = PLECanvasSize::toPixels(height,
                                  d->yResolution->value(),
                                  PLECanvasSize::sizeUnit(d->sizeUnitsWidget->currentText()),
                                  PLECanvasSize::resolutionUnit(d->resolutionUnitsWidget->currentText()));
    d->HEIGHT = height;
    this->setHorizontal(d->WIDTH > d->HEIGHT);
    this->setVertical(d->WIDTH < d->HEIGHT);
    d->updateSizeLabel();
}

void PLECanvasSizeWidget::xResolutionChanged(double xResolution)
{
    PLECanvasSize::SizeUnits sizeUnit = PLECanvasSize::sizeUnit(d->sizeUnitsWidget->currentText());
    if (sizeUnit == PLECanvasSize::Pixels)
        return;
    PLECanvasSize::ResolutionUnits resolutionUnit = PLECanvasSize::resolutionUnit(d->resolutionUnitsWidget->currentText());
    qreal resolutionFactor = PLECanvasSize::resolutionUnitFactor(resolutionUnit);
    int width = PLECanvasSize::toPixels(d->xSize->value(),
                                     xResolution,
                                     PLECanvasSize::sizeUnit(d->sizeUnitsWidget->currentText()),
                                     PLECanvasSize::resolutionUnit(d->resolutionUnitsWidget->currentText()));
    d->WIDTH = width;
    d->WIDTH_RES = xResolution * resolutionFactor;
    d->updateSizeLabel();
}

void PLECanvasSizeWidget::yResolutionChanged(double yResolution)
{
    PLECanvasSize::SizeUnits sizeUnit = PLECanvasSize::sizeUnit(d->sizeUnitsWidget->currentText());
    if (sizeUnit == PLECanvasSize::Pixels)
        return;
    PLECanvasSize::ResolutionUnits resolutionUnit = PLECanvasSize::resolutionUnit(d->resolutionUnitsWidget->currentText());
    qreal resolutionFactor = PLECanvasSize::resolutionUnitFactor(resolutionUnit);
    int height = PLECanvasSize::toPixels(d->ySize->value(),
                                      yResolution,
                                      PLECanvasSize::sizeUnit(d->sizeUnitsWidget->currentText()),
                                      PLECanvasSize::resolutionUnit(d->resolutionUnitsWidget->currentText()));
    d->HEIGHT = height;
    d->HEIGHT_RES = yResolution * resolutionFactor;
    d->updateSizeLabel();
}

} // namespace PhotoLayoutsEditor
