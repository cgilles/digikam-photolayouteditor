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

#include "toolsdockwidget.h"

// Qt includes

#include <QDebug>
#include <QButtonGroup>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QScrollArea>
#include <QIcon>

// Local includes

#include "plecanvasedittool.h"
#include "effectseditortool.h"
#include "texteditortool.h"
#include "borderedittool.h"
#include "zoomtool.h"

namespace PhotoLayoutsEditor
{

class ToolsDockWidget::ToolsDockWidgetPrivate
{
    ToolsDockWidgetPrivate()
      : zoom_tool(nullptr),
        canvas_tool(nullptr),
        effects_tool(nullptr),
        text_tool(nullptr),
        border_tool(nullptr),
        formLayout(nullptr),
        toolArea(nullptr)
    {
    }

    ZoomTool*          zoom_tool;
    PLECanvasEditTool* canvas_tool;
    EffectsEditorTool* effects_tool;
    TextEditorTool*    text_tool;
    BorderEditTool*    border_tool;
    QGridLayout*       formLayout;
    QScrollArea*       toolArea;

    friend class ToolsDockWidget;
};

ToolsDockWidget* ToolsDockWidget::m_instance = nullptr;

class MyStackedLayout : public QStackedLayout
{
    public:

        explicit MyStackedLayout(QWidget* parent = nullptr) : QStackedLayout(parent)
        {
        }

        QSize sizeHint() const override
        {
            QSize s = QStackedLayout::sizeHint();
            s.setHeight(this->currentWidget()->sizeHint().height());
            return s;
        }

        QSize minimumSize() const override
        {
            return sizeHint();
        }
};

ToolsDockWidget* ToolsDockWidget::instance(QWidget* parent)
{
    if (!m_instance)
        m_instance = new ToolsDockWidget(parent);

    return m_instance;
}

ToolsDockWidget::ToolsDockWidget(QWidget* parent)
    : QDockWidget(QObject::tr("Tools"),parent),
//      m_has_selection(false),
      m_current_item(nullptr),
      m_scene(nullptr),
      d(new ToolsDockWidgetPrivate)
{
    this->setFeatures(QDockWidget::DockWidgetMovable);
    this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* widget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(widget);
    //layout->setSizeConstraint(QLayout::SetMinimumSize);

    // tools buttons layout
    d->formLayout = new QGridLayout();
    //formLayout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->addLayout(d->formLayout);

    // stacked widget (with tools widgets)
    d->toolArea = new QScrollArea(widget);
    //sa->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    d->toolArea->setFrameShape(QFrame::NoFrame);
    d->toolArea->setWidgetResizable(true);
    d->toolArea->setWidget(nullptr);
    layout->addWidget(d->toolArea,1);

    QButtonGroup* group = new QButtonGroup(widget);

    // Selection tool

    m_tool_pointer = new QPushButton(QIcon(QLatin1String(":/pointer.png")), QString(), widget);
    m_tool_pointer->setToolTip(QObject::tr("Tool which allows one to select and move images on canvas. Any other operations are disabled."));
    m_tool_pointer->setIconSize(QSize(24,24));
    m_tool_pointer->setFixedSize(32,32);
    m_tool_pointer->setCheckable(true);
    m_tool_pointer->setFlat(true);
    group->addButton(m_tool_pointer);

    connect(m_tool_pointer, SIGNAL(toggled(bool)),
            this, SLOT(setPointerToolVisible(bool)));

    // View tool
    m_tool_hand = new QPushButton(QIcon(QLatin1String(":/hand.png")), QString(), widget);
    m_tool_hand->setToolTip(QObject::tr("This tool allows one to view whole canvas in read-only mode. Only scrolling and zooming are available."));
    m_tool_hand->setIconSize(QSize(24,24));
    m_tool_hand->setFixedSize(32,32);
    m_tool_hand->setCheckable(true);
    m_tool_hand->setFlat(true);
    group->addButton(m_tool_hand);

    connect(m_tool_hand, SIGNAL(toggled(bool)),
            this, SLOT(setHandToolVisible(bool)));

    // Zoom tool
    m_tool_zoom = new QPushButton(QIcon(QLatin1String(":/zoom.png")), QString(), widget);
    m_tool_zoom->setToolTip(QObject::tr("This tool allows one to zoom canvas to fit it to the application window or users preferences."));
    m_tool_zoom->setIconSize(QSize(24,24));
    m_tool_zoom->setFixedSize(32,32);
    m_tool_zoom->setCheckable(true);
    m_tool_zoom->setFlat(true);
    group->addButton(m_tool_zoom);

    connect(m_tool_zoom, SIGNAL(toggled(bool)),
            this, SLOT(setZoomWidgetVisible(bool)));

    // PLECanvas edit tool
    m_canvas_button = new QPushButton(QIcon(QLatin1String(":/tool_canvas.png")), QString(), widget);
    m_canvas_button->setToolTip(QObject::tr("This tool allows you to edit canvas properties like size and background."));
    m_canvas_button->setIconSize(QSize(24,24));
    m_canvas_button->setFixedSize(32,32);
    m_canvas_button->setCheckable(true);
    m_canvas_button->setFlat(true);
    group->addButton(m_canvas_button);

    connect(m_canvas_button, SIGNAL(toggled(bool)),
            this, SLOT(setPLECanvasWidgetVisible(bool)));

    // Text tool
    m_text_button = new QPushButton(QIcon(QLatin1String(":/tool_text.png")), QString(), 
    widget);
    m_text_button->setToolTip(QObject::tr("This tool allows you to write text on the canvas. It's simple - just click on the canvas where you want to add some text and write it!"));

    m_text_button->setIconSize(QSize(24,24));
    m_text_button->setFixedSize(32,32);
    m_text_button->setCheckable(true);
    m_text_button->setFlat(true);
    group->addButton(m_text_button);

    connect(m_text_button, SIGNAL(toggled(bool)),
            this, SLOT(setTextWidgetVisible(bool)));

    // Rotate tool
    m_rotate_button = new QPushButton(QIcon(QLatin1String(":/tool_rotate.png")), QString(), widget);
    m_rotate_button->setToolTip(QObject::tr("This tool allows you to rotate items on your canvas."));
    m_rotate_button->setIconSize(QSize(24,24));
    m_rotate_button->setFixedSize(32,32);
    m_rotate_button->setCheckable(true);
    m_rotate_button->setFlat(true);
    group->addButton(m_rotate_button);

    connect(m_rotate_button, SIGNAL(toggled(bool)),
            this, SLOT(setRotateWidgetVisible(bool)));

    // Scale tool
    m_scale_button = new QPushButton(QIcon(QLatin1String(":/tool_scale4.png")), QString(), widget);
    m_scale_button->setToolTip(QObject::tr("This tool allows you to scale items on your canvas."));
    m_scale_button->setIconSize(QSize(24,24));
    m_scale_button->setFixedSize(32,32);
    m_scale_button->setCheckable(true);
    m_scale_button->setFlat(true);
    group->addButton(m_scale_button);

    connect(m_scale_button, SIGNAL(toggled(bool)),
            this, SLOT(setScaleWidgetVisible(bool)));

    // Crop tool
    m_crop_button = new QPushButton(QIcon(QLatin1String(":/tool_cropt.png")), QString(), widget);
    m_crop_button->setToolTip(QObject::tr("This tool allows you to crop items."));
    m_crop_button->setIconSize(QSize(24,24));
    m_crop_button->setFixedSize(32,32);
    m_crop_button->setCheckable(true);
    m_crop_button->setFlat(true);
    group->addButton(m_crop_button);

    connect(m_crop_button, SIGNAL(toggled(bool)),
            this, SLOT(setCropWidgetVisible(bool)));

    // Photo effects tool
    m_effects_button = new QPushButton(QIcon(QLatin1String(":/tool_effects.png")), QString(), widget);
    m_effects_button->setToolTip(QObject::tr("This tool allows you to edit existing effects of your photo layers and add some new one."));
    m_effects_button->setIconSize(QSize(24,24));
    m_effects_button->setFixedSize(32,32);
    m_effects_button->setCheckable(true);
    m_effects_button->setFlat(true);
    group->addButton(m_effects_button);

    connect(m_effects_button, SIGNAL(toggled(bool)),
            this, SLOT(setEffectsWidgetVisible(bool)));

    // Border edit tool
    m_tool_border = new QPushButton(QIcon(QLatin1String(":/tool_border.png")), QString(), widget);
    m_tool_border->setIconSize(QSize(24,24));
    m_tool_border->setFixedSize(32,32);
    m_tool_border->setCheckable(true);
    m_tool_border->setFlat(true);
    group->addButton(m_tool_border);

    connect(m_tool_border, SIGNAL(toggled(bool)),
            this, SLOT(setBordersWidgetVisible(bool)));

    // Spacer
    d->formLayout->setContentsMargins(QMargins());
    d->formLayout->setSpacing(0);

    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);
    widget->setLayout(layout);
    //widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    this->setWidget(widget);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    this->setMinimumWidth(235);

    setDefaultTool();
}

ToolsDockWidget::~ToolsDockWidget()
{
    m_instance = nullptr;
    delete d;
}

void ToolsDockWidget::setDefaultTool()
{
    m_tool_hand->setChecked(true);
    this->setHandToolVisible(true);
}

void ToolsDockWidget::setScene(PLEScene* scene)
{
    if (scene)
        this->connect(scene, SIGNAL(destroyed()), this, SLOT(setScene()));

    if (sender() && !scene && this->m_scene)
        return;

    m_scene = scene;
    QWidget* w = d->toolArea->widget();
    AbstractTool * tool = dynamic_cast<AbstractTool*>(w);

    if (tool)
        tool->setScene(m_scene);
}

void ToolsDockWidget::itemSelected(AbstractPhoto* photo)
{
    qDebug() << "itemSelected" << (QGraphicsItem*)photo;
    m_current_item = photo;
    QWidget* w = d->toolArea->widget();

    if (!w)
        return;

    AbstractItemsTool * tool =qobject_cast<AbstractItemsTool*>(w);

    if (tool)
        tool->setCurrentItem(photo);

    qDebug() << tool;
}

void ToolsDockWidget::mousePositionChoosen(const QPointF& position)
{
    QWidget* w = d->toolArea->widget();

    if (!w)
        return;

    AbstractItemsTool * tool =qobject_cast<AbstractItemsTool*>(w);

    if (tool)
        tool->setMousePosition(position);
}

void ToolsDockWidget::emitNewItemCreated(AbstractPhoto* item)
{
    if (!item)
        return;

    Q_EMIT newItemCreated(item);
}

void ToolsDockWidget::setPointerToolVisible(bool isSelected)
{
    m_tool_pointer->setChecked(isSelected);

    if (isSelected)
    {
        d->toolArea->setWidget(nullptr);
        this->unsetCursor();
        Q_EMIT requireMultiSelection();
        Q_EMIT pointerToolSelected();
    }
}

void ToolsDockWidget::setHandToolVisible(bool isSelected)
{
    m_tool_hand->setChecked(isSelected);

    if (isSelected)
    {
        d->toolArea->setWidget(nullptr);
        this->unsetCursor();
        Q_EMIT requireMultiSelection();
        Q_EMIT handToolSelected();
    }
}

void ToolsDockWidget::setZoomWidgetVisible(bool isVisible)
{
    if (d->zoom_tool)
    {
        d->zoom_tool->deleteLater();
        d->zoom_tool = nullptr;
    }

    m_tool_zoom->setChecked(isVisible);

    Q_EMIT zoomToolSelectionChanged(isVisible);

    if (isVisible)
    {
        d->zoom_tool = new ZoomTool(nullptr, d->toolArea);
        d->zoom_tool->setScene(m_scene);
        d->toolArea->setWidget(d->zoom_tool);
        Q_EMIT requireSingleSelection();
        Q_EMIT zoomToolSelected();
    }
}

void ToolsDockWidget::setPLECanvasWidgetVisible(bool isVisible)
{
    if (d->canvas_tool)
    {
        d->canvas_tool->deleteLater();
        d->canvas_tool = nullptr;
    }

    m_canvas_button->setChecked(isVisible);

    Q_EMIT canvasToolSelectionChanged(isVisible);

    if (isVisible)
    {
        d->canvas_tool = new PLECanvasEditTool(nullptr, d->toolArea);
        d->canvas_tool->setScene(m_scene);
        d->toolArea->setWidget(d->canvas_tool);
        Q_EMIT requireMultiSelection();
        Q_EMIT canvasToolSelected();
    }
}

void ToolsDockWidget::setEffectsWidgetVisible(bool isVisible)
{
    if (d->effects_tool)
    {
        d->effects_tool->deleteLater();
        d->effects_tool = nullptr;
    }

    m_effects_button->setChecked(isVisible);

    Q_EMIT effectsToolSelectionChanged(isVisible);

    if (isVisible)
    {
        d->effects_tool = new EffectsEditorTool(nullptr, d->toolArea);
        d->effects_tool->setScene(m_scene);
        d->effects_tool->setCurrentItem(m_current_item);
        d->toolArea->setWidget(d->effects_tool);
        Q_EMIT requireSingleSelection();
        Q_EMIT effectsToolSelected();
    }
}

void ToolsDockWidget::setTextWidgetVisible(bool isVisible)
{
    if (d->text_tool)
    {
        d->text_tool->deleteLater();
        d->text_tool = nullptr;
    }

    m_text_button->setChecked(isVisible);

    Q_EMIT textToolSelectionChanged(isVisible);

    if (isVisible)
    {
        d->text_tool = new TextEditorTool(nullptr, d->toolArea);

        connect(d->text_tool, SIGNAL(itemCreated(AbstractPhoto*)),
                this, SLOT(emitNewItemCreated(AbstractPhoto*)));

        d->text_tool->setScene(m_scene);
        d->text_tool->setCurrentItem(m_current_item);
        d->toolArea->setWidget(d->text_tool);
        Q_EMIT requireSingleSelection();
        Q_EMIT textToolSelected();
    }
}

void ToolsDockWidget::setRotateWidgetVisible(bool isVisible)
{
    Q_EMIT rotateToolSelectionChanged(isVisible);
    m_rotate_button->setChecked(isVisible);

    if (isVisible)
    {
        d->toolArea->setWidget(nullptr);
        Q_EMIT requireSingleSelection();
        Q_EMIT rotateToolSelected();
    }
}

void ToolsDockWidget::setScaleWidgetVisible(bool isVisible)
{
    Q_EMIT scaleToolSelectionChanged(isVisible);
    m_scale_button->setChecked(isVisible);

    if (isVisible)
    {
        d->toolArea->setWidget(nullptr);
        Q_EMIT requireSingleSelection();
        Q_EMIT scaleToolSelected();
    }
}

void ToolsDockWidget::setCropWidgetVisible(bool isVisible)
{
    Q_EMIT cropToolSelectionChanged(isVisible);
    m_crop_button->setChecked(isVisible);

    if (isVisible)
    {
        d->toolArea->setWidget(nullptr);
        Q_EMIT requireSingleSelection();
        Q_EMIT cropToolSelected();
    }
}

void ToolsDockWidget::setBordersWidgetVisible(bool isVisible)
{
    if (d->border_tool)
    {
        d->border_tool->deleteLater();
        d->border_tool = nullptr;
    }

    m_tool_border->setChecked(isVisible);

    Q_EMIT borderToolSelectionChanged(isVisible);

    if (isVisible)
    {
        d->border_tool = new BorderEditTool(nullptr, d->toolArea);
        d->border_tool->setScene(m_scene);
        d->border_tool->setCurrentItem(m_current_item);
        d->toolArea->setWidget(d->border_tool);
        Q_EMIT requireSingleSelection();
        Q_EMIT borderToolSelected();
    }
}

void ToolsDockWidget::resizeEvent(QResizeEvent* event)
{
    QList<QWidget*> l;
    l << m_tool_pointer
      << m_tool_hand
      << m_tool_zoom
      << m_canvas_button
      << m_text_button
      << m_rotate_button
      << m_scale_button
      << m_crop_button
      << m_effects_button
      << m_tool_border;

    foreach (QWidget* const w, l)
        d->formLayout->removeWidget(w);

    int width = 0;
    int col = 0, row = 0;

    foreach (QWidget* const w, l)
    {
        width += w->size().width();

        if (row < (int)(width / event->size().width()) )
        {
            d->formLayout->setColumnStretch(col, 1);
            row += 1;
            col = 0;
            width = row * event->size().width() + w->size().width();
        }

        d->formLayout->setColumnStretch(col, 0);
        d->formLayout->addWidget(w, row, col++, Qt::AlignCenter);
        d->formLayout->setRowStretch(row, 0);
    }

    if (!row)
        d->formLayout->setColumnStretch(col, 1);
}

} // namespace PhotoLayoutsEditor
