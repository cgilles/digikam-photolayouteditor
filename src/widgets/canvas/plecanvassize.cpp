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

#include "plecanvassize.h"

namespace PhotoLayoutsEditor
{

QMap<PLECanvasSize::ResolutionUnits,qreal> PLECanvasSize::resolution_factors;
QMap<PLECanvasSize::ResolutionUnits,QString> PLECanvasSize::resolution_names;
QMap<PLECanvasSize::SizeUnits,qreal> PLECanvasSize::size_factors;
QMap<PLECanvasSize::SizeUnits,QString> PLECanvasSize::size_names;

void PLECanvasSize::prepare_maps()
{
    if (resolution_factors.isEmpty() || resolution_names.isEmpty())
    {
        resolution_factors.clear();
        resolution_names.clear();

        resolution_factors.insert(UnknownResolutionUnit, 0);
        resolution_factors.insert(PixelsPerMilimeter, 25.4);
        resolution_factors.insert(PixelsPerCentimeter, 2.54);
        resolution_factors.insert(PixelsPerInch, 1);
        resolution_factors.insert(PixelsPerPoint, 72.27);
        resolution_factors.insert(PixelsPerPicas, 6.0225);

        resolution_names.insert(PixelsPerMilimeter, QLatin1String("px/mm"));
        resolution_names.insert(PixelsPerCentimeter, QLatin1String("px/cm"));
        resolution_names.insert(PixelsPerInch, QLatin1String("px/in"));
        resolution_names.insert(PixelsPerPoint, QLatin1String("px/pt"));
        resolution_names.insert(PixelsPerPicas, QLatin1String("px/pc"));
    }

    if (size_factors.isEmpty() || size_names.isEmpty())
    {
        size_factors.clear();
        size_names.clear();

        size_factors.insert(UnknownSizeUnit, 0);
        size_factors.insert(Milimeters, 25.4);
        size_factors.insert(Centimeters, 2.54);
        size_factors.insert(Inches, 1);
        size_factors.insert(Points, 72.27);
        size_factors.insert(Picas, 6.0225);

        size_names.insert(Pixels, QLatin1String("px"));
        size_names.insert(Milimeters, QLatin1String("mm"));
        size_names.insert(Centimeters, QLatin1String("cm"));
        size_names.insert(Inches, QLatin1String("in"));
        size_names.insert(Points, QLatin1String("pt"));
        size_names.insert(Picas, QLatin1String("pc"));
    }
}

QList<qreal> PLECanvasSize::resolutionUnitsFactors()
{
    prepare_maps();
    return resolution_factors.values();
}

QList<QString> PLECanvasSize::resolutionUnitsNames()
{
    prepare_maps();
    return resolution_names.values();
}

QList<PLECanvasSize::ResolutionUnits> PLECanvasSize::resolutionUnits()
{
    prepare_maps();
    return resolution_factors.keys();
}

qreal PLECanvasSize::resolutionUnitFactor(PLECanvasSize::ResolutionUnits unit)
{
    prepare_maps();
    return resolution_factors.value(unit, 0);
}

qreal PLECanvasSize::resolutionUnitFactor(QString unitName)
{
    prepare_maps();
    return resolution_factors.value( resolution_names.key(unitName, UnknownResolutionUnit) );
}

QString PLECanvasSize::resolutionUnitName(PLECanvasSize::ResolutionUnits unit)
{
    prepare_maps();
    return resolution_names.value(unit);
}

QString PLECanvasSize::resolutionUnitName(qreal factor)
{
    prepare_maps();
    return resolution_names.value(resolution_factors.key(factor));
}

PLECanvasSize::ResolutionUnits PLECanvasSize::resolutionUnit(qreal factor)
{
    prepare_maps();
    return resolution_factors.key(factor, UnknownResolutionUnit);
}

PLECanvasSize::ResolutionUnits PLECanvasSize::resolutionUnit(QString name)
{
    prepare_maps();
    return resolution_names.key(name, UnknownResolutionUnit);
}

qreal PLECanvasSize::resolutionConvert(qreal value, ResolutionUnits from, ResolutionUnits to)
{
    qreal fromFactor = resolutionUnitFactor(from);
    qreal toFactor = resolutionUnitFactor(to);
    if (!fromFactor || !toFactor)
        return value;
    value /= fromFactor;
    value *= toFactor;
    return value;
}

QList<qreal> PLECanvasSize::sizeUnitsFactors()
{
    prepare_maps();
    return size_factors.values();
}

QList<QString> PLECanvasSize::sizeUnitsNames()
{
    prepare_maps();
    return size_names.values();
}

QList<PLECanvasSize::SizeUnits> PLECanvasSize::sizeUnits()
{
    prepare_maps();
    return size_factors.keys();
}

qreal PLECanvasSize::sizeUnitFactor(PLECanvasSize::SizeUnits unit)
{
    prepare_maps();
    return size_factors.value(unit);
}

qreal PLECanvasSize::sizeUnitFactor(QString unitName)
{
    prepare_maps();
    return size_factors.value(size_names.key(unitName, UnknownSizeUnit));
}

QString PLECanvasSize::sizeUnitName(PLECanvasSize::SizeUnits unit)
{
    prepare_maps();
    return size_names.value(unit);
}

QString PLECanvasSize::sizeUnitName(qreal factor)
{
    prepare_maps();
    return size_names.value(size_factors.key(factor));
}

PLECanvasSize::SizeUnits PLECanvasSize::sizeUnit(qreal factor)
{
    prepare_maps();
    return size_factors.key(factor, UnknownSizeUnit);
}

PLECanvasSize::SizeUnits PLECanvasSize::sizeUnit(QString name)
{
    prepare_maps();
    return size_names.key(name, UnknownSizeUnit);
}

qreal PLECanvasSize::sizeConvert(qreal value, SizeUnits from, SizeUnits to)
{
    qreal fromFactor = sizeUnitFactor(from);
    qreal toFactor = sizeUnitFactor(to);
    if (!fromFactor || !toFactor)
        return value;
    value /= fromFactor;
    value *= toFactor;
    return value;
}

int PLECanvasSize::toPixels(qreal value, qreal resolution, SizeUnits sUnit, ResolutionUnits rUnit)
{
    if (sUnit == Pixels)
        return value;
    qreal result = (resolutionUnitFactor(rUnit) * resolution * value)
                   / sizeUnitFactor(sUnit);
    return qRound(result);
}

qreal PLECanvasSize::fromPixels(int pixels, qreal resolution, SizeUnits sUnit, ResolutionUnits rUnit)
{
    qreal sizeFactor = sizeUnitFactor(sUnit);
    qreal resolutionFactor = resolutionUnitFactor(rUnit);
    return (pixels * sizeFactor) / (resolution * resolutionFactor);
}

PLECanvasSize::PLECanvasSize()
{
    prepare_maps();
    m_size = QSizeF();
    m_size_unit = UnknownSizeUnit;
    m_resolution = QSizeF();
    m_resolution_unit = UnknownResolutionUnit;
}

PLECanvasSize::PLECanvasSize(const QSizeF& size, SizeUnits sUnit, const QSizeF& resolution, ResolutionUnits rUnit)
{
    prepare_maps();
    m_size = size;
    m_size_unit = sUnit;
    m_resolution = resolution;
    m_resolution_unit = rUnit;
}

QSizeF PLECanvasSize::size() const
{
    return m_size;
}

void PLECanvasSize::setSize(const QSizeF& size)
{
    if (!size.isValid())
        return;
    m_size = size;
}

QSizeF PLECanvasSize::size(SizeUnits unit) const
{
    QSizeF result;
    result.setWidth( toPixels(m_size.width(), m_resolution.width(), m_size_unit, m_resolution_unit) );
    result.setHeight( toPixels(m_size.height(), m_resolution.height(), m_size_unit, m_resolution_unit) );
    if (unit != Pixels)
    {
        result.setWidth( fromPixels(result.width(), m_resolution.width(), unit, m_resolution_unit) );
        result.setHeight( fromPixels(result.height(), m_resolution.height(), unit, m_resolution_unit) );
    }
    return result;
}

PLECanvasSize::SizeUnits PLECanvasSize::sizeUnit() const
{
    return m_size_unit;
}

void PLECanvasSize::setSizeUnit(SizeUnits unit)
{
    if (unit < Pixels || unit > Picas)
        return;
    m_size_unit = unit;
}

QSizeF PLECanvasSize::resolution() const
{
    return m_resolution;
}

QSizeF PLECanvasSize::resolution(ResolutionUnits unit) const
{
    if (!this->isValid())
        return QSizeF();

    QSizeF result = m_resolution;
    if (m_resolution_unit != PLECanvasSize::PixelsPerInch)
    {
        qreal factor = PLECanvasSize::resolutionUnitFactor(m_resolution_unit);
        result.setWidth(result.width() * factor);
        result.setHeight(result.height() * factor);
    }

    if (unit != m_resolution_unit &&
            unit != PLECanvasSize::UnknownResolutionUnit)
    {
        qreal factor = PLECanvasSize::resolutionUnitFactor(unit);
        result.setWidth(result.width() / factor);
        result.setHeight(result.height() / factor);
    }
    return result;
}

void PLECanvasSize::setResolution(const QSizeF& resolution)
{
    if (!resolution.isValid())
        return;
    m_resolution = resolution;
}

PLECanvasSize::ResolutionUnits PLECanvasSize::resolutionUnit() const
{
    return m_resolution_unit;
}

void PLECanvasSize::setResolutionUnit(ResolutionUnits unit)
{
    if (unit < PixelsPerMilimeter || unit > PixelsPerPicas)
        return;
    m_resolution_unit = unit;
}

bool PLECanvasSize::isValid() const
{
    return m_size.isValid() &&
            m_resolution.isValid() &&
            (m_size_unit != UnknownSizeUnit) &&
            (m_resolution_unit != UnknownResolutionUnit);
}

bool PLECanvasSize::operator ==(const PLECanvasSize& size) const
{
    return this->m_size == size.m_size &&
           this->m_size_unit == size.m_size_unit &&
           this->m_resolution == size.m_resolution &&
           this->m_resolution_unit == size.m_resolution_unit;
}

bool PLECanvasSize::operator !=(const PLECanvasSize& size) const
{
    return !(*this == size);
}

} // namespace PhotoLayoutsEditor
