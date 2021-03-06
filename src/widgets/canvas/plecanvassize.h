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

#ifndef CANVAS_SIZE_H
#define CANVAS_SIZE_H

// Qt includes

#include <QString>
#include <QMap>
#include <QSizeF>

namespace PhotoLayoutsEditor
{

class PLECanvasSizeDialog;

class PLECanvasSize
{
public:

    enum ResolutionUnits
    {
        UnknownResolutionUnit,
        PixelsPerMilimeter,
        PixelsPerCentimeter,
        PixelsPerInch,
        PixelsPerPoint,
        PixelsPerPicas
    };

    enum SizeUnits
    {
        UnknownSizeUnit,
        Pixels,
        Milimeters,
        Centimeters,
        Inches,
        Points,
        Picas
    };

    static QList<qreal> resolutionUnitsFactors();
    static QList<QString> resolutionUnitsNames();
    static QList<ResolutionUnits> resolutionUnits();
    static qreal resolutionUnitFactor(ResolutionUnits unit);
    static qreal resolutionUnitFactor(const QString& unitName);
    static QString resolutionUnitName(ResolutionUnits unit);
    static QString resolutionUnitName(qreal factor);
    static ResolutionUnits resolutionUnit(qreal factor);
    static ResolutionUnits resolutionUnit(const QString& name);
    static qreal resolutionConvert(qreal value, ResolutionUnits from, ResolutionUnits to);

    static QList<qreal> sizeUnitsFactors();
    static QList<QString> sizeUnitsNames();
    static QList<SizeUnits> sizeUnits();
    static qreal sizeUnitFactor(SizeUnits unit);
    static qreal sizeUnitFactor(const QString& unitName);
    static QString sizeUnitName(SizeUnits unit);
    static QString sizeUnitName(qreal factor);
    static SizeUnits sizeUnit(qreal factor);
    static SizeUnits sizeUnit(const QString& name);
    static qreal sizeConvert(qreal value, SizeUnits from, SizeUnits to);

    static int toPixels(qreal value, qreal resolution, SizeUnits sUnit, ResolutionUnits rUnit);
    static qreal fromPixels(int pixels, qreal resolution, SizeUnits sUnit, ResolutionUnits rUnit);

    PLECanvasSize();
    PLECanvasSize(const QSizeF& size, SizeUnits sUnit, const QSizeF& resolution, ResolutionUnits rUnit);

    QSizeF size() const;
    void setSize(const QSizeF& size);
    QSizeF size(SizeUnits unit) const;
    SizeUnits sizeUnit() const;
    void setSizeUnit(SizeUnits unit);
    QSizeF resolution() const;
    QSizeF resolution(ResolutionUnits unit) const;
    void setResolution(const QSizeF& resolution);
    ResolutionUnits resolutionUnit() const;
    void setResolutionUnit(ResolutionUnits unit);
    bool isValid() const;

    bool operator ==(const PLECanvasSize& size) const;
    bool operator !=(const PLECanvasSize& size) const;

private:

    static QMap<ResolutionUnits,qreal> resolution_factors;
    static QMap<ResolutionUnits,QString> resolution_names;

    static QMap<SizeUnits,qreal> size_factors;
    static QMap<SizeUnits,QString> size_names;

    static void prepare_maps();

    SizeUnits m_size_unit;
    QSizeF m_size;
    ResolutionUnits m_resolution_unit;
    QSizeF m_resolution;

    friend class PLECanvasSizeDialog;
};

} // namespace PhotoLayoutsEditor

#endif // CANVAS_SIZE_H
