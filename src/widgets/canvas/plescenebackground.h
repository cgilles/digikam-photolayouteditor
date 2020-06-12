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

#ifndef SCENE_BACKGROUND_H
#define SCENE_BACKGROUND_H

// Qt includes

#include <QBrush>
#include <QGraphicsItem>
#include <QDomDocument>

namespace PhotoLayoutsEditor
{

class PLESceneBackgroundLoader;

class PLESceneBackground : public QObject,
                           public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

    QRectF m_rect;
    QBrush m_first_brush;
    QBrush m_second_brush;

    // Image background specific data
    QImage m_image;
    Qt::Alignment m_image_align;
    Qt::AspectRatioMode m_image_aspect_ratio;
    QSize m_image_size;
    bool m_image_repeat;

    // For painting/rendering purpose
    QImage m_temp_image;

    class BackgroundImageChangedCommand;
    class BackgroundFirstBrushChangeCommand;
    class BackgroundSecondBrushChangeCommand;

public:

    explicit PLESceneBackground(QGraphicsScene* scene = nullptr);
    virtual QRectF boundingRect() const override;

    void setSecondColor(const QColor& color);
    void setSolidColor(const QColor& color);
    void setPattern(const QColor& firstColor, const QColor& secondColor, Qt::BrushStyle patternStyle);
    void setImage(const QImage& image, const QColor& backgroundColor, Qt::Alignment align, Qt::AspectRatioMode aspectRatio, bool repeat);
    void setImage(const QImage& image, const QColor& backgroundColor, Qt::Alignment align, const QSize& fixedSize, bool repeat);

    bool isColor() const;
    bool isGradient() const;
    bool isImage() const;
    bool isPattern() const;

    QDomElement toSvg(QDomDocument& document) const;
    bool fromSvg(QDomElement& element);

    QColor firstColor() const;
    QColor secondColor() const;
    Qt::BrushStyle pattern() const;
    QImage image() const;
    Qt::Alignment imageAlignment() const;
    Qt::AspectRatioMode imageAspectRatio() const;
    QSize imageSize() const;
    bool imageRepeated() const;

Q_SIGNALS:

    void changed();

protected:

    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void render(QPainter* painter, const QRect& rect);

protected Q_SLOTS:

    void render();

private:

    void sceneChanged();

private Q_SLOTS:

    void sceneRectChanged(const QRectF& sceneRect);

    friend class PLESceneBackgroundLoader;
    friend class BackgroundImageChangedCommand;
    friend class BackgroundFirstBrushChangeCommand;
    friend class BackgroundSecondBrushChangeCommand;
};

} // namespace PhotoLayoutsEditor

#endif // SCENE_BACKGROUND_H
