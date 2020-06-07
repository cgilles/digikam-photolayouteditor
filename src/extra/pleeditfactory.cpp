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

#if defined(__GNUC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif

#include "pleeditfactory.h"

// Qt includes

#include <QHBoxLayout>
#include <QSpinBox>
#include <QSlider>
#include <QDebug>

namespace PhotoLayoutsEditor
{

QSpinBoxFactory::QSpinBoxFactory(QObject* const parent)
    : QtSpinBoxFactory(parent)
{
}

QWidget* QSpinBoxFactory::createEditor(QtIntPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtSpinBoxFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QSliderFactory::QSliderFactory(QObject* const parent)
    : QtSliderFactory(parent)
{
}

QWidget* QSliderFactory::createEditor(QtIntPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtSliderFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QScrollBarFactory::QScrollBarFactory(QObject* const parent)
    : QtScrollBarFactory(parent)
{
}

QWidget* QScrollBarFactory::createEditor(QtIntPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtScrollBarFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QCheckBoxFactory::QCheckBoxFactory(QObject* const parent)
    : QtCheckBoxFactory(parent)
{
}

QWidget* QCheckBoxFactory::createEditor(QtBoolPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtCheckBoxFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QDoubleSpinBoxFactory::QDoubleSpinBoxFactory(QObject* const parent)
    : QtDoubleSpinBoxFactory(parent)
{
    connect(this, SIGNAL(destroyed(QObject*)), this, SIGNAL(editingFinished()));
}

QWidget* QDoubleSpinBoxFactory::createEditor(QtDoublePropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtDoubleSpinBoxFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QLineEditFactory::QLineEditFactory(QObject* const parent)
    : QtLineEditFactory(parent)
{
}

QWidget* QLineEditFactory::createEditor(QtStringPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtLineEditFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));
    return widget;
}

// ---------------------------------------------------------------------

QDateEditFactory::QDateEditFactory(QObject* const parent) :
    QtDateEditFactory(parent)
{
}

QWidget* QDateEditFactory::createEditor(QtDatePropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtDateEditFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QTimeEditFactory::QTimeEditFactory(QObject* const parent)
    : QtTimeEditFactory(parent)
{
}

QWidget* QTimeEditFactory::createEditor(QtTimePropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtTimeEditFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));
    return widget;
}

// ---------------------------------------------------------------------

QDateTimeEditFactory::QDateTimeEditFactory(QObject* const parent) :
    QtDateTimeEditFactory(parent)
{
}

QWidget* QDateTimeEditFactory::createEditor(QtDateTimePropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtDateTimeEditFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QKeySequenceEditorFactory::QKeySequenceEditorFactory(QObject* const parent)
    : QtKeySequenceEditorFactory(parent)
{
}

QWidget* QKeySequenceEditorFactory::createEditor(QtKeySequencePropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtKeySequenceEditorFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QCharEditorFactory::QCharEditorFactory(QObject* const parent) :
    QtCharEditorFactory(parent)
{
}

QWidget* QCharEditorFactory::createEditor(QtCharPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtCharEditorFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QEnumEditorFactory::QEnumEditorFactory(QObject* const parent)
    : QtEnumEditorFactory(parent)
{
}

QWidget* QEnumEditorFactory::createEditor(QtEnumPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtEnumEditorFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QCursorEditorFactory::QCursorEditorFactory(QObject* const parent)
    : QtCursorEditorFactory(parent)
{
}

QWidget* QCursorEditorFactory::createEditor(QtCursorPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtCursorEditorFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QColorEditorFactory::QColorEditorFactory(QObject* const parent) :
    QtColorEditorFactory(parent)
{
}

QWidget* QColorEditorFactory::createEditor(QtColorPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtColorEditorFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QFontEditorFactory::QFontEditorFactory(QObject* const parent)
    : QtFontEditorFactory(parent)
{
}

QWidget* QFontEditorFactory::createEditor(QtFontPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtFontEditorFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QVariantEditorFactory::QVariantEditorFactory(QObject* const parent)
    : QtVariantEditorFactory(parent)
{
}

QWidget* QVariantEditorFactory::createEditor(QtVariantPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtVariantEditorFactory::createEditor(manager, property, parent);
    connect(widget ,SIGNAL(destroyed()), this, SLOT(emitEditingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

QSliderEditFactory::QSliderEditFactory(QObject* const parent)
    : QtAbstractEditorFactory<QtIntPropertyManager>(parent)
{
    originalFactory = new QtSliderFactory(this);
}

void QSliderEditFactory::connectPropertyManager(QtIntPropertyManager* manager)
{
    this->addPropertyManager(manager);
    originalFactory->addPropertyManager(manager);
}

QWidget* QSliderEditFactory::createEditor(QtIntPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QtAbstractEditorFactoryBase* const base = originalFactory;

    QWidget* w = base->createEditor(property,parent);

    if (!w)
    {
        return nullptr;
    }

    QSlider* const slider = qobject_cast<QSlider*>(w);

    if (!slider)
    {
        return nullptr;
    }

    w = new QWidget(parent);
    slider->setParent(w);
    QSpinBox* const spinbox   = new QSpinBox(w);
    spinbox->setMaximum(manager->maximum(property));
    spinbox->setMinimum(manager->minimum(property));
    spinbox->setValue(manager->value(property));
    QHBoxLayout* const layout = new QHBoxLayout();
    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);
    layout->addWidget(slider,1);
    layout->addWidget(spinbox,0);
    w->setLayout(layout);

    createdEditors[property].append(w);
    editorToProperty[w] = property;

    connect(slider, SIGNAL(valueChanged(int)), spinbox, SLOT(setValue(int)));
    connect(spinbox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    connect(w, SIGNAL(destroyed(QObject*)), this, SLOT(slotEditorDestroyed(QObject*)));

    return w;
}

void QSliderEditFactory::disconnectPropertyManager(QtIntPropertyManager* manager)
{
    this->removePropertyManager(manager);
    originalFactory->removePropertyManager(manager);
}

void QSliderEditFactory::slotEditorDestroyed(QObject* object)
{
    emit editingFinished();
    QMap<QWidget*, QtProperty*>::ConstIterator itEditor = editorToProperty.constBegin();

    while (itEditor != editorToProperty.constEnd())
    {
        if (itEditor.key() == object)
        {
            QWidget* const editor      = itEditor.key();
            QtProperty* const property = itEditor.value();
            editorToProperty.remove(editor);
            createdEditors[property].removeAll(editor);

            if (createdEditors[property].isEmpty())
            {
                createdEditors.remove(property);
            }

            return;
        }

        itEditor++;
    }
}

} // namespace PhotoLayoutsEditor

#if defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif
