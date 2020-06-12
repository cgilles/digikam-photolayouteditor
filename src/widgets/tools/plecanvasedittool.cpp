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

/**
  *
  * In next version:
  *
  *     TODO : Gradient fill background:
  *               - create gradient editor widget
  *               - create MouseListener button
  *
  */

#include "plecanvasedittool.h"

// Qt includes

#include <QApplication>
#include <QFormLayout>
#include <QGroupBox>
#include <QStackedLayout>
#include <QSpinBox>
#include <QImage>
#include <QImageReader>
#include <QCheckBox>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QStandardPaths>

// digiKam includes

#include "dcolorselector.h"
#include "imagedialog.h"

// Local incudes

#include "plescenebackground.h"
#include "mousepresslistener.h"
#include "patternscombobox.h"
#include "plesceneborder.h"

using namespace Digikam;

namespace PhotoLayoutsEditor
{

class PLECanvasEditToolPrivate
{
    enum BackgroundType
    {
        ColorFill,
        PatternFill,
        GradientFill,
        ImageFill
    };

    enum ScallingType
    {
        Expanded = 1,
        Scaled = 2,
        Manual = 4
    };

    explicit PLECanvasEditToolPrivate(PLECanvasEditTool * /*parent*/) :
//        m_parent(parent),
//        background_gradient_widget(nullptr),
        background_type_widget(nullptr),
        background_widgets(nullptr),
        background_color_widget(nullptr),
        background_color(nullptr),
        background_image_widget(nullptr),
        backgroundImageFormLayout(nullptr),
//        background_image_file(nullptr),
        background_image_label(nullptr),
        background_image_scalling(nullptr),
        background_image_tiled(nullptr),
        background_alignBox(nullptr),
        background_image_HAlign(nullptr),
        background_image_VAlign(nullptr),
        background_sizeBox(nullptr),
        background_image_width(nullptr),
        background_image_height(nullptr),
        background_image_color(nullptr),
        border_image_label(nullptr),
        background_pattern_widget(nullptr),
        background_pattern_color1(nullptr),
        background_pattern_color2(nullptr),
        background_pattern_type(nullptr)
    {
        background_types.insert(QObject::tr("Color"),    ColorFill);
        background_types.insert(QObject::tr("Image"),    ImageFill);
        background_types.insert(QObject::tr("Pattern"),  PatternFill);
//        background_types.insert(QObject::tr("Gradient"), GradientFill);

        background_image_scalling_map.insert(Expanded, QObject::tr("Expanded"));
        background_image_scalling_map.insert(Scaled, QObject::tr("Scaled"));
        background_image_scalling_map.insert(Manual, QObject::tr("Fixed size"));

        background_image_Halignment_map.insert(Qt::AlignHCenter, QObject::tr("Center"));
        background_image_Halignment_map.insert(Qt::AlignLeft, QObject::tr("Left"));
        background_image_Halignment_map.insert(Qt::AlignRight, QObject::tr("Right"));

        background_image_Valignment_map.insert(Qt::AlignVCenter, QObject::tr("Center"));
        background_image_Valignment_map.insert(Qt::AlignTop, QObject::tr("Top"));
        background_image_Valignment_map.insert(Qt::AlignBottom, QObject::tr("Bottom"));

        background_image_empty_pixmap = QPixmap(150, 100);
        background_image_empty_pixmap.fill(Qt::transparent);
        QPainter p(&background_image_empty_pixmap);
        p.drawText(background_image_empty_pixmap.rect(), Qt::AlignCenter, QObject::tr("Click here to set an image"));

        border_image_empty_pixmap = QPixmap(150, 100);
        border_image_empty_pixmap.fill(Qt::transparent);
        QPainter p2(&border_image_empty_pixmap);
        p2.drawText(border_image_empty_pixmap.rect(), Qt::AlignCenter, QObject::tr("Click here to set an image"));
    }

    void setImageWidgetsEnabled(bool enabled)
    {
        background_image_scalling->setEnabled(enabled);
        background_image_tiled->setEnabled(enabled);
        background_image_HAlign->setEnabled(enabled);
        background_image_VAlign->setEnabled(enabled);
        background_image_width->setEnabled(enabled);
        background_image_height->setEnabled(enabled);
    }

//    PLECanvasEditTool * m_parent;

    QMap<QString, BackgroundType> background_types;
    QComboBox* background_type_widget;
    QStackedLayout * background_widgets;

    QWidget* background_color_widget;
    DColorSelector * background_color;

//    QWidget* background_gradient_widget;

    QWidget* background_image_widget;
    QFormLayout * backgroundImageFormLayout;
//    QUrlRequester * background_image_file;
    QPixmap background_image_empty_pixmap;
    QPushButton* background_image_label;
    QComboBox* background_image_scalling;
    QMap<ScallingType, QString> background_image_scalling_map;
    QCheckBox * background_image_tiled;
    QGroupBox * background_alignBox;
    QComboBox* background_image_HAlign;
    QMap<Qt::Alignment, QString> background_image_Halignment_map;
    QComboBox* background_image_VAlign;
    QMap<Qt::Alignment, QString> background_image_Valignment_map;
    QGroupBox * background_sizeBox;
    QSpinBox * background_image_width;
    QSpinBox * background_image_height;
    DColorSelector * background_image_color;
    QImage m_image;

    QPixmap border_image_empty_pixmap;
    QPushButton* border_image_label;
    QImage m_border_image;

    QWidget* background_pattern_widget;
    DColorSelector * background_pattern_color1;
    DColorSelector * background_pattern_color2;
    PatternsComboBox * background_pattern_type;

    MousePressListener mouse_listener;

    friend class PLECanvasEditTool;
};

PLECanvasEditTool::PLECanvasEditTool(PLEScene* scene, QWidget* parent)
    : AbstractTool(scene, PLECanvas::Viewing, parent),
      d(new PLECanvasEditToolPrivate(this)),
      hold_update(false)
{
    setupGUI();
}

PLECanvasEditTool::~PLECanvasEditTool()
{
    delete d;
}

void PLECanvasEditTool::backgroundTypeChanged(const QString& typeName)
{
    qDebug() << typeName;
    PLECanvasEditToolPrivate::BackgroundType bt = d->background_types.value(typeName);
    switch (bt)
    {
        case PLECanvasEditToolPrivate::ColorFill:
            colorBackgroundSelected();
            break;
        case PLECanvasEditToolPrivate::GradientFill:
            gradientBackgroundSelected();
            break;
        case PLECanvasEditToolPrivate::ImageFill:
            imageBackgroundSelected();
            break;
        case PLECanvasEditToolPrivate::PatternFill:
            patternBackgroundSelected();
            break;
    }
}

void PLECanvasEditTool::sceneChange()
{
    PLEScene* scene = this->scene();
    if (!scene)
        return;
}

void PLECanvasEditTool::sceneChanged()
{
    PLEScene* scene = this->scene();
    if (!scene)
        return;

    PLESceneBackground* background = scene->background();
    this->connect(background, SIGNAL(changed()), this, SLOT(updateWidgets()));

    this->updateWidgets();
}

void PLECanvasEditTool::colorBackgroundSelected()
{
    d->background_widgets->setCurrentWidget(d->background_color_widget);

    if (this->hold_update)
        return;

    PLESceneBackground* background = scene()->background();
    background->setSolidColor(d->background_color->color());
}

void PLECanvasEditTool::gradientBackgroundSelected()
{
    }

void PLECanvasEditTool::imageBackgroundSelected()
{
    d->background_widgets->setCurrentWidget(d->background_image_widget);
    if (d->m_image.isNull())
        return;
    PLEScene* scene = this->scene();
    if (!scene)
        return;

    if (this->hold_update)
        return;

    this->setImageBackground();
}

void PLECanvasEditTool::patternBackgroundSelected()
{
    d->background_widgets->setCurrentWidget(d->background_pattern_widget);

    if (this->hold_update)
        return;

    this->setPatternBackground();
}

void PLECanvasEditTool::solidColorChanged(const QColor& color)
{
    PLEScene* scene = this->scene();
    if (!scene)
        return;
    scene->background()->setSolidColor(color);
}

void PLECanvasEditTool::imageBackgroundColorChanged(const QColor& color)
{
    if (this->hold_update)
        return;
    PLEScene* scene = this->scene();
    if (!scene)
        return;
    scene->background()->setSecondColor(color);
}

void PLECanvasEditTool::patternFirstColorChanged(const QColor& /*color*/)
{
    if (this->hold_update)
        return;
    PLEScene* scene = this->scene();
    if (!scene)
        return;
    this->setPatternBackground();
}

void PLECanvasEditTool::patternSecondColorChanged(const QColor& /*color*/)
{
    if (this->hold_update)
        return;
    PLEScene* scene = this->scene();
    if (!scene)
        return;
    this->setPatternBackground();
}

void PLECanvasEditTool::patternStyleChanged(Qt::BrushStyle /*patternStyle*/)
{
    if (this->hold_update)
        return;
    PLEScene* scene = this->scene();
    if (!scene)
        return;
    this->setPatternBackground();
}

void PLECanvasEditTool::imageUrlRequest()
{
    static QString startUrl(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    QUrl url = ImageDialog::getImageURL(this, QUrl::fromLocalFile(startUrl), QString());
    if (url.isEmpty())
        return;

    bool valid = false;
    QImageReader ir(url.toLocalFile());

    if (ir.canRead())
    {
        if (ir.read(&d->m_image))
        {
            QPixmap tempPX = QPixmap::fromImage(d->m_image.scaled(QSize(150,150), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            d->background_image_label->setIcon(QIcon(tempPX));
            d->background_image_label->setIconSize(tempPX.size());

            Q_UNUSED(this->hold_update);
            this->hold_update = true;
            QSizeF sceneSize = scene()->sceneRect().size();
            QSize  imageSize = d->m_image.size();
            bool widthSmaller = sceneSize.width() > imageSize.width();
            bool heightSmaller = sceneSize.height()> imageSize.height();
            d->background_image_width->setValue(imageSize.width());
            d->background_image_height->setValue(imageSize.height());
            if (widthSmaller && heightSmaller)
                d->background_image_scalling->setCurrentText( d->background_image_scalling_map.value(PLECanvasEditToolPrivate::Manual) );
            else if (widthSmaller || heightSmaller)
                d->background_image_scalling->setCurrentText( d->background_image_scalling_map.value(PLECanvasEditToolPrivate::Expanded) );
            else
                d->background_image_scalling->setCurrentText( d->background_image_scalling_map.value(PLECanvasEditToolPrivate::Scaled) );

            Q_UNUSED(this->hold_update);
            this->hold_update = false;
            this->setImageBackground();

            valid = true;
        }
    }

    if (!valid)
    {
        QMessageBox::critical(qApp->activeWindow(), QObject::tr("Error"), QObject::tr("Invalid or unsupported image file."));
        d->background_image_label->setIcon(QIcon(d->background_image_empty_pixmap));
        d->background_image_label->setIconSize(d->background_image_empty_pixmap.size());
    }
    d->setImageWidgetsEnabled(valid);

    startUrl = QFileInfo(url.toLocalFile()).path();
}

void PLECanvasEditTool::borderImageUrlRequest()
{
    static QString startUrl(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    QUrl url = ImageDialog::getImageURL(this, QUrl::fromLocalFile(startUrl), QString());
    if (url.isEmpty())
        return;

    bool valid = false;
    QImageReader ir(url.toLocalFile());

    if (ir.canRead())
    {
        if (ir.read(&d->m_border_image))
        {
            QPixmap tempPX = QPixmap::fromImage(d->m_border_image.scaled(QSize(150,150), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            d->border_image_label->setIcon(QIcon(tempPX));
            d->border_image_label->setIconSize(tempPX.size());

            this->setImageBorder();
            valid = true;
        }
    }

    if (!valid)
    {
        QMessageBox::critical(qApp->activeWindow(), QObject::tr("Error"), QObject::tr("Invalid or unsupported image file."));
        d->border_image_label->setIcon(QIcon(d->border_image_empty_pixmap));
        d->border_image_label->setIconSize(d->border_image_empty_pixmap.size());
    }

    startUrl = QFileInfo(url.toLocalFile()).path();
}

void PLECanvasEditTool::imageScallingChanged(const QString& scallingName)
{
    PLECanvasEditToolPrivate::ScallingType st = d->background_image_scalling_map.key(scallingName);
    d->background_sizeBox->setVisible(st == PLECanvasEditToolPrivate::Manual);
    this->setImageBackground();
}

void PLECanvasEditTool::imageTiledChanged(int /*state*/)
{
    this->setImageBackground();
}

void PLECanvasEditTool::imageHorizontalAlignmentChanged(int /*index*/)
{
    this->setImageBackground();
}

void PLECanvasEditTool::imageVerticalAlignmentChanged(int /*index*/)
{
    this->setImageBackground();
}

void PLECanvasEditTool::imageWidthChanged()
{
    static int width = -1;
    if (width != d->background_image_width->value())
        this->setImageBackground();
    width = d->background_image_width->value();
}

void PLECanvasEditTool::imageHeightChanged()
{
    static int height = -1;
    if (height != d->background_image_height->value())
        this->setImageBackground();
    height = d->background_image_height->value();
}

void PLECanvasEditTool::setImageBackground()
{
    if (d->m_image.isNull() || this->hold_update)
        return;

    PLESceneBackground* background = scene()->background();
    bool tiled = d->background_image_tiled->isChecked();
    Qt::Alignment alignment = d->background_image_Halignment_map.key( d->background_image_HAlign->currentText() ) |
                              d->background_image_Valignment_map.key( d->background_image_VAlign->currentText() );
    PLECanvasEditToolPrivate::ScallingType scalingMode = d->background_image_scalling_map.key(d->background_image_scalling->currentText());
    if (scalingMode == PLECanvasEditToolPrivate::Manual)
    {
        QSize size(d->background_image_width->value(),
                   d->background_image_height->value());
        background->setImage(d->m_image, d->background_image_color->color(), alignment, size, tiled);
    }
    else
    {
        Qt::AspectRatioMode aspectRatio;
        switch (scalingMode)
        {
            case PLECanvasEditToolPrivate::Expanded:
                aspectRatio = Qt::KeepAspectRatioByExpanding;
                break;
            default:
                aspectRatio = Qt::KeepAspectRatio;
        }
        background->setImage(d->m_image, d->background_image_color->color(), alignment, aspectRatio, tiled);
    }
}

void PLECanvasEditTool::setPatternBackground()
{
    if (this->hold_update)
        return;

    PLEScene* scene = this->scene();
    scene->background()->setPattern(d->background_pattern_color1->color(),
                                    d->background_pattern_color2->color(),
                                    d->background_pattern_type->pattern());
}

void PLECanvasEditTool::setImageBorder()
{
    if (d->m_border_image.isNull() || this->hold_update)
        return;

    PLESceneBorder* border = scene()->border();
    if (border)
        border->setImage(d->m_border_image);
}

void PLECanvasEditTool::setupGUI()
{
    QVBoxLayout* layout = new QVBoxLayout();
    this->setLayout(layout);

    // PLECanvas border group
    QGroupBox * borderGroup = new QGroupBox(QObject::tr("Border"), this);
    layout->addWidget(borderGroup);
    QFormLayout * borderLayout = new QFormLayout();
    borderGroup->setLayout(borderLayout);
    {
        d->border_image_label = new QPushButton(borderGroup);
        borderLayout->addRow(d->border_image_label);
        d->border_image_label->setFlat(true);
        d->border_image_label->setFocusPolicy(Qt::NoFocus);
        d->border_image_label->setIcon(QIcon(d->border_image_empty_pixmap));
        d->border_image_label->setIconSize(d->border_image_empty_pixmap.size());
    }

    // PLECanvas background group
    QGroupBox * backgroundGroup = new QGroupBox(QObject::tr("Background"), this);
    layout->addWidget(backgroundGroup);
    QFormLayout * backgroundLayout = new QFormLayout();
    backgroundGroup->setLayout(backgroundLayout);
    {
        // Background type widget
        d->background_type_widget = new QComboBox(backgroundGroup);
        d->background_type_widget->addItems( d->background_types.keys() );
        backgroundLayout->addRow(QObject::tr("Type"), d->background_type_widget);

        d->background_widgets = new QStackedLayout();
        backgroundLayout->addRow(d->background_widgets);

        // Color type widget
        d->background_color_widget = new QWidget(backgroundGroup);
        QFormLayout * colorFormLayout = new QFormLayout();
        d->background_color_widget->setLayout(colorFormLayout);
        d->background_widgets->addWidget(d->background_color_widget);
        d->background_color = new DColorSelector(d->background_color_widget);
        //d->background_color->setAlphaChannelEnabled(true);
        d->background_color->setColor(Qt::transparent);
        colorFormLayout->addRow(QObject::tr("Color"), d->background_color);

        // Image type widget
        d->background_image_widget = new QWidget(backgroundGroup);
        d->backgroundImageFormLayout = new QFormLayout();
        d->background_image_widget->setLayout(d->backgroundImageFormLayout);
        d->background_widgets->addWidget(d->background_image_widget);

        d->background_image_label = new QPushButton(d->background_image_widget);
        d->backgroundImageFormLayout->addRow(d->background_image_label);
        d->background_image_label->setFlat(true);
        d->background_image_label->setFocusPolicy(Qt::NoFocus);
        d->background_image_label->setIcon(QIcon(d->background_image_empty_pixmap));
        d->background_image_label->setIconSize(d->background_image_empty_pixmap.size());

        d->background_image_scalling = new QComboBox(d->background_image_widget);
        d->background_image_scalling->addItems(d->background_image_scalling_map.values());
        d->background_image_scalling->setEnabled(false);
        d->backgroundImageFormLayout->addRow(QObject::tr("Scaling"), d->background_image_scalling);
        d->background_image_scalling->setCurrentIndex(-1);

        d->background_image_tiled = new QCheckBox(d->background_image_widget);
        d->background_image_tiled->setEnabled(false);
        d->backgroundImageFormLayout->addRow(QObject::tr("Tiled"), d->background_image_tiled);

        d->background_alignBox = new QGroupBox(QObject::tr("Alignment"));
        QFormLayout * alignForm = new QFormLayout();
        d->background_alignBox->setLayout(alignForm);
            d->background_image_HAlign = new QComboBox(d->background_image_widget);
            d->background_image_HAlign->addItems(d->background_image_Halignment_map.values());
            d->background_image_HAlign->setEnabled(false);
            alignForm->addRow(QObject::tr("Horizontal"), d->background_image_HAlign);
            d->background_image_VAlign = new QComboBox(d->background_image_widget);
            d->background_image_VAlign->addItems(d->background_image_Valignment_map.values());
            d->background_image_VAlign->setEnabled(false);
            alignForm->addRow(QObject::tr("Vertical"), d->background_image_VAlign);
        d->backgroundImageFormLayout->addRow(d->background_alignBox);

        d->background_sizeBox = new QGroupBox(QObject::tr("Size"));
        QFormLayout * sizeForm = new QFormLayout();
        d->background_sizeBox->setLayout(sizeForm);
            d->background_image_width = new QSpinBox(d->background_image_widget);
            d->background_image_width->setEnabled(false);
            d->background_image_width->setMinimum(1);
            d->background_image_width->setMaximum(99999);
            sizeForm->addRow(QObject::tr("Width"), d->background_image_width);
            d->background_image_height = new QSpinBox(d->background_image_widget);
            d->background_image_height->setEnabled(false);
            d->background_image_height->setMinimum(1);
            d->background_image_height->setMaximum(99999);
            sizeForm->addRow(QObject::tr("Height"), d->background_image_height);
        d->backgroundImageFormLayout->addRow(d->background_sizeBox);

        d->background_image_color = new DColorSelector(d->background_image_widget);
        d->background_image_color->setColor(Qt::transparent);
        //d->background_image_color->setAlphaChannelEnabled(true);
        d->backgroundImageFormLayout->addRow(QObject::tr("Color"), d->background_image_color);

        // Pattern type widget
        d->background_pattern_widget = new QWidget(backgroundGroup);
        QFormLayout * patternFormLayout = new QFormLayout();
        d->background_pattern_widget->setLayout(patternFormLayout);
        d->background_widgets->addWidget(d->background_pattern_widget);
        d->background_pattern_color1 = new DColorSelector(d->background_pattern_widget);
        d->background_pattern_color1->setColor(Qt::transparent);
        //d->background_pattern_color1->setAlphaChannelEnabled(true);
        patternFormLayout->addRow(QObject::tr("Color 1"), d->background_pattern_color1);
        d->background_pattern_color2 = new DColorSelector(d->background_pattern_widget);
        d->background_pattern_color2->setColor(Qt::transparent);
        //d->background_pattern_color2->setAlphaChannelEnabled(true);
        patternFormLayout->addRow(QObject::tr("Color 2"), d->background_pattern_color2);
        d->background_pattern_type = new PatternsComboBox(d->background_pattern_widget);
        patternFormLayout->addRow(QObject::tr("Pattern"), d->background_pattern_type);
    }

    connect(d->background_type_widget, SIGNAL(currentIndexChanged(QString)), this, SLOT(backgroundTypeChanged(QString)));
    connect(d->background_color, SIGNAL(signalColorSelected(QColor)), this, SLOT(solidColorChanged(QColor)));
    connect(d->background_image_label, SIGNAL(clicked()), this, SLOT(imageUrlRequest()));
    connect(d->background_image_scalling, SIGNAL(currentIndexChanged(QString)), this, SLOT(imageScallingChanged(QString)));
    connect(d->background_image_tiled, SIGNAL(stateChanged(int)), this, SLOT(imageTiledChanged(int)));
    connect(d->background_image_HAlign, SIGNAL(currentIndexChanged(int)), this, SLOT(imageHorizontalAlignmentChanged(int)));
    connect(d->background_image_VAlign, SIGNAL(currentIndexChanged(int)), this, SLOT(imageVerticalAlignmentChanged(int)));
    connect(d->background_image_width, SIGNAL(editingFinished()), this, SLOT(imageWidthChanged()));
    connect(d->background_image_height, SIGNAL(editingFinished()), this, SLOT(imageHeightChanged()));
    connect(d->background_image_color, SIGNAL(signalColorSelected(QColor)), this, SLOT(imageBackgroundColorChanged(QColor)));
    connect(d->background_pattern_color1, SIGNAL(signalColorSelected(QColor)), this, SLOT(patternFirstColorChanged(QColor)));
    connect(d->background_pattern_color2, SIGNAL(signalColorSelected(QColor)), this, SLOT(patternSecondColorChanged(QColor)));
    connect(d->background_pattern_type, SIGNAL(currentPatternChanged(Qt::BrushStyle)), this, SLOT(patternStyleChanged(Qt::BrushStyle)));
    connect(&(d->mouse_listener), SIGNAL(mousePressed(QPointF)), this, SLOT(readMousePosition(QPointF)));
    connect(d->border_image_label, SIGNAL(clicked()), this, SLOT(borderImageUrlRequest()));

    if (scene())
    {
        PLEScene* scene = this->scene();
        PLESceneBackground* background = scene->background();
        if (background->isColor())
            this->colorBackgroundSelected();
        else if (background->isGradient())
            this->gradientBackgroundSelected();
        else if (background->isImage())
            this->imageBackgroundSelected();
        else if (background->isPattern())
            this->patternBackgroundSelected();

        PLESceneBorder* border = scene->border();
        d->m_border_image = border->image();
        if (!d->m_border_image.isNull())
        {
            QPixmap tempPX = QPixmap::fromImage(d->m_border_image.scaled(QSize(150,150), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            d->border_image_label->setIcon(QIcon(tempPX));
            d->border_image_label->setIconSize(tempPX.size());
        }
    }
}

void PLECanvasEditTool::readMousePosition(const QPointF& scenePos)
{
    qDebug() << scenePos;
}

void PLECanvasEditTool::updateWidgets()
{
    PLEScene* scene = this->scene();
    if (!scene)
        return;

    PLESceneBackground* background = scene->background();
    if (!background)
        return;

    PLESceneBorder* border = scene->border();
    if (!border)
        return;

    d->m_border_image = border->image();
    if (!d->m_border_image.isNull())
    {
        QPixmap tempPX = QPixmap::fromImage(d->m_border_image.scaled(QSize(150,150), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        d->border_image_label->setIcon(QIcon(tempPX));
        d->border_image_label->setIconSize(tempPX.size());
    }

    Q_UNUSED(this->hold_update);
    this->hold_update = true;

    if (background->isPattern())
    {
        d->background_widgets->setCurrentWidget(d->background_pattern_widget);
        d->background_pattern_type->setPattern(background->pattern());
        d->background_pattern_color1->setColor(background->firstColor());
        d->background_pattern_color2->setColor(background->secondColor());
        d->background_type_widget->setCurrentText(d->background_types.key(PLECanvasEditToolPrivate::PatternFill));
    }
    else if (background->isImage())
    {
        d->background_widgets->setCurrentWidget(d->background_image_widget);
        d->m_image = background->image();
        QPixmap tempPX = QPixmap::fromImage(d->m_image.scaled(QSize(150,150), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        d->background_image_label->setIcon(QIcon(tempPX));
        d->background_image_label->setIconSize(tempPX.size());
        d->background_image_HAlign->setCurrentText( d->background_image_Halignment_map.value(background->imageAlignment() & (Qt::AlignHorizontal_Mask)) );
        d->background_image_VAlign->setCurrentText( d->background_image_Valignment_map.value(background->imageAlignment() & (Qt::AlignVertical_Mask)) );
        d->background_image_width->setValue( background->imageSize().width() );
        d->background_image_height->setValue( background->imageSize().height() );
        PLECanvasEditToolPrivate::ScallingType scallingType;
        switch (background->imageAspectRatio())
        {
            case Qt::KeepAspectRatioByExpanding:
                scallingType = PLECanvasEditToolPrivate::Expanded;
                break;
            case Qt::KeepAspectRatio:
                scallingType = PLECanvasEditToolPrivate::Scaled;
                break;
            default:
                scallingType = PLECanvasEditToolPrivate::Manual;
        }
        d->background_image_scalling->setCurrentText( d->background_image_scalling_map.value(scallingType) );
        d->background_image_tiled->setChecked( background->imageRepeated() );
        d->background_image_color->setColor( background->secondColor() );
        d->background_type_widget->setCurrentText(d->background_types.key(PLECanvasEditToolPrivate::ImageFill));
        d->setImageWidgetsEnabled(!d->m_image.isNull());
    }
    else
    {
        d->background_widgets->setCurrentWidget(d->background_color_widget);
        d->background_color->setColor(background->firstColor());
        d->background_type_widget->setCurrentText(d->background_types.key(PLECanvasEditToolPrivate::ColorFill));
    }

    Q_UNUSED(this->hold_update);
    this->hold_update = false;
}

} // namespace PhotoLayoutsEditor
