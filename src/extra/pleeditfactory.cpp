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

PLESpinBoxFactory::PLESpinBoxFactory(QObject* const parent)
    : QtSpinBoxFactory(parent)
{
}

QWidget* PLESpinBoxFactory::createEditor(QtIntPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtSpinBoxFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLESliderFactory::PLESliderFactory(QObject* const parent)
    : QtSliderFactory(parent)
{
}

QWidget* PLESliderFactory::createEditor(QtIntPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtSliderFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLEScrollBarFactory::PLEScrollBarFactory(QObject* const parent)
    : QtScrollBarFactory(parent)
{
}

QWidget* PLEScrollBarFactory::createEditor(QtIntPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtScrollBarFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLECheckBoxFactory::PLECheckBoxFactory(QObject* const parent)
    : QtCheckBoxFactory(parent)
{
}

QWidget* PLECheckBoxFactory::createEditor(QtBoolPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtCheckBoxFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLEDoubleSpinBoxFactory::PLEDoubleSpinBoxFactory(QObject* const parent)
    : QtDoubleSpinBoxFactory(parent)
{
}

QWidget* PLEDoubleSpinBoxFactory::createEditor(QtDoublePropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtDoubleSpinBoxFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLELineEditFactory::PLELineEditFactory(QObject* const parent)
    : QtLineEditFactory(parent)
{
}

QWidget* PLELineEditFactory::createEditor(QtStringPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtLineEditFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLEDateEditFactory::PLEDateEditFactory(QObject* const parent) :
    QtDateEditFactory(parent)
{
}

QWidget* PLEDateEditFactory::createEditor(QtDatePropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtDateEditFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLETimeEditFactory::PLETimeEditFactory(QObject* const parent)
    : QtTimeEditFactory(parent)
{
}

QWidget* PLETimeEditFactory::createEditor(QtTimePropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtTimeEditFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLEDateTimeEditFactory::PLEDateTimeEditFactory(QObject* const parent) :
    QtDateTimeEditFactory(parent)
{
}

QWidget* PLEDateTimeEditFactory::createEditor(QtDateTimePropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtDateTimeEditFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLEKeySequenceEditorFactory::PLEKeySequenceEditorFactory(QObject* const parent)
    : QtKeySequenceEditorFactory(parent)
{
}

QWidget* PLEKeySequenceEditorFactory::createEditor(QtKeySequencePropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtKeySequenceEditorFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLECharEditorFactory::PLECharEditorFactory(QObject* const parent) :
    QtCharEditorFactory(parent)
{
}

QWidget* PLECharEditorFactory::createEditor(QtCharPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtCharEditorFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLEEnumEditorFactory::PLEEnumEditorFactory(QObject* const parent)
    : QtEnumEditorFactory(parent)
{
}

QWidget* PLEEnumEditorFactory::createEditor(QtEnumPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtEnumEditorFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLECursorEditorFactory::PLECursorEditorFactory(QObject* const parent)
    : QtCursorEditorFactory(parent)
{
}

QWidget* PLECursorEditorFactory::createEditor(QtCursorPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtCursorEditorFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLEColorEditorFactory::PLEColorEditorFactory(QObject* const parent) :
    QtColorEditorFactory(parent)
{
}

QWidget* PLEColorEditorFactory::createEditor(QtColorPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtColorEditorFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLEFontEditorFactory::PLEFontEditorFactory(QObject* const parent)
    : QtFontEditorFactory(parent)
{
}

QWidget* PLEFontEditorFactory::createEditor(QtFontPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtFontEditorFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLEVariantEditorFactory::PLEVariantEditorFactory(QObject* const parent)
    : QtVariantEditorFactory(parent)
{
}

QWidget* PLEVariantEditorFactory::createEditor(QtVariantPropertyManager* manager, QtProperty* property, QWidget* parent)
{
    QWidget* const widget = QtVariantEditorFactory::createEditor(manager, property, parent);

    connect(widget, SIGNAL(destroyed()),
            this, SIGNAL(editingFinished()));

    return widget;
}

// ---------------------------------------------------------------------

PLESliderEditFactory::PLESliderEditFactory(QObject* const parent)
    : QtAbstractEditorFactory<QtIntPropertyManager>(parent)
{
    originalFactory = new QtSliderFactory(this);
}

void PLESliderEditFactory::connectPropertyManager(QtIntPropertyManager* manager)
{
    this->addPropertyManager(manager);
    originalFactory->addPropertyManager(manager);
}

QWidget* PLESliderEditFactory::createEditor(QtIntPropertyManager* manager, QtProperty* property, QWidget* parent)
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

    connect(slider, SIGNAL(valueChanged(int)),
            spinbox, SLOT(setValue(int)));

    connect(spinbox, SIGNAL(valueChanged(int)),
            slider, SLOT(setValue(int)));

    connect(w, SIGNAL(destroyed(QObject*)), this,
            SLOT(slotEditorDestroyed(QObject*)));

    return w;
}

void PLESliderEditFactory::disconnectPropertyManager(QtIntPropertyManager* manager)
{
    this->removePropertyManager(manager);
    originalFactory->removePropertyManager(manager);
}

void PLESliderEditFactory::slotEditorDestroyed(QObject* object)
{
    Q_EMIT editingFinished();
    QMap<QWidget*, QtProperty*>::const_iterator itEditor = editorToProperty.constBegin();

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

        ++itEditor;
    }
}

} // namespace PhotoLayoutsEditor

#if defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif
