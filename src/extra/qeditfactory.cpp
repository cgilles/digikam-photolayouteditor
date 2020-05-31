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

#include "qeditfactory.h"

// Qt includes

#include <QHBoxLayout>
#include <QSpinBox>
#include <QSlider>
#include <QDebug>

namespace PhotoLayoutsEditor
{

QSpinBoxFactory::QSpinBoxFactory(QObject* parent)
    : QtSpinBoxFactory(parent)
{
}

QWidget * QSpinBoxFactory::createEditor(QtIntPropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtSpinBoxFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QSliderFactory::QSliderFactory(QObject * parent) :
    QtSliderFactory(parent)
{
}

QWidget * QSliderFactory::createEditor(QtIntPropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtSliderFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QScrollBarFactory::QScrollBarFactory(QObject * parent) :
    QtScrollBarFactory(parent)
{
}

QWidget * QScrollBarFactory::createEditor(QtIntPropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtScrollBarFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QCheckBoxFactory::QCheckBoxFactory(QObject * parent) :
    QtCheckBoxFactory(parent)
{
}

QWidget * QCheckBoxFactory::createEditor(QtBoolPropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtCheckBoxFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QDoubleSpinBoxFactory::QDoubleSpinBoxFactory(QObject * parent) :
    QtDoubleSpinBoxFactory(parent)
{
}

QWidget * QDoubleSpinBoxFactory::createEditor(QtDoublePropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtDoubleSpinBoxFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QLineEditFactory::QLineEditFactory(QObject * parent) :
    QtLineEditFactory(parent)
{
}

QWidget * QLineEditFactory::createEditor(QtStringPropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtLineEditFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QDateEditFactory::QDateEditFactory(QObject * parent) :
    QtDateEditFactory(parent)
{
}

QWidget * QDateEditFactory::createEditor(QtDatePropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtDateEditFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QTimeEditFactory::QTimeEditFactory(QObject * parent) :
    QtTimeEditFactory(parent)
{
}

QWidget * QTimeEditFactory::createEditor(QtTimePropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtTimeEditFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QDateTimeEditFactory::QDateTimeEditFactory(QObject * parent) :
    QtDateTimeEditFactory(parent)
{
}

QWidget * QDateTimeEditFactory::createEditor(QtDateTimePropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtDateTimeEditFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QKeySequenceEditorFactory::QKeySequenceEditorFactory(QObject * parent) :
    QtKeySequenceEditorFactory(parent)
{
}

QWidget * QKeySequenceEditorFactory::createEditor(QtKeySequencePropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtKeySequenceEditorFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QCharEditorFactory::QCharEditorFactory(QObject * parent) :
    QtCharEditorFactory(parent)
{
}

QWidget * QCharEditorFactory::createEditor(QtCharPropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtCharEditorFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QEnumEditorFactory::QEnumEditorFactory(QObject * parent) :
    QtEnumEditorFactory(parent)
{
}

QWidget * QEnumEditorFactory::createEditor(QtEnumPropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtEnumEditorFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QCursorEditorFactory::QCursorEditorFactory(QObject * parent) :
    QtCursorEditorFactory(parent)
{
}

QWidget * QCursorEditorFactory::createEditor(QtCursorPropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtCursorEditorFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QColorEditorFactory::QColorEditorFactory(QObject * parent) :
    QtColorEditorFactory(parent)
{
}

QWidget * QColorEditorFactory::createEditor(QtColorPropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtColorEditorFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QFontEditorFactory::QFontEditorFactory(QObject * parent) :
    QtFontEditorFactory(parent)
{
}

QWidget * QFontEditorFactory::createEditor(QtFontPropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QWidget * widget = QtFontEditorFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QVariantEditorFactory::QVariantEditorFactory(QObject *parent) :
    QtVariantEditorFactory(parent)
{}

QWidget * QVariantEditorFactory::createEditor(QtVariantPropertyManager *manager, QtProperty *property, QWidget *parent)
{
    QWidget * widget = QtVariantEditorFactory::createEditor(manager,property,parent);
    connect(widget,SIGNAL(destroyed()),this,SLOT(emitEditingFinished()));
    return widget;
}

QSliderEditFactory::QSliderEditFactory(QObject *parent) :
    QtAbstractEditorFactory<QtIntPropertyManager>(parent)
{
    originalFactory = new QtSliderFactory(this);
}

void QSliderEditFactory::connectPropertyManager(QtIntPropertyManager * manager)
{
    this->addPropertyManager(manager);
    originalFactory->addPropertyManager(manager);
}

QWidget * QSliderEditFactory::createEditor(QtIntPropertyManager * manager, QtProperty * property, QWidget * parent)
{
    QtAbstractEditorFactoryBase* const base = originalFactory;

    QWidget* w = base->createEditor(property,parent);

    if (!w)
        return 0;

    QSlider* slider = qobject_cast<QSlider*>(w);

    if (!slider)
        return 0;

    w = new QWidget(parent);
    slider->setParent(w);
    QSpinBox* spinbox = new QSpinBox(w);
    spinbox->setMaximum(manager->maximum(property));
    spinbox->setMinimum(manager->minimum(property));
    spinbox->setValue(manager->value(property));
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);
    layout->addWidget(slider,1);
    layout->addWidget(spinbox,0);
    w->setLayout(layout);

    createdEditors[property].append(w);
    editorToProperty[w] = property;

    connect(slider,SIGNAL(valueChanged(int)),spinbox,SLOT(setValue(int)));
    connect(spinbox,SIGNAL(valueChanged(int)),slider,SLOT(setValue(int)));
    connect(w,SIGNAL(destroyed(QObject*)),this,SLOT(slotEditorDestroyed(QObject*)));

    return w;
}

void QSliderEditFactory::disconnectPropertyManager(QtIntPropertyManager * manager)
{
    this->removePropertyManager(manager);
    originalFactory->removePropertyManager(manager);
}

void QSliderEditFactory::slotEditorDestroyed(QObject *object)
{
    emit editingFinished();
    QMap<QWidget *, QtProperty *>::ConstIterator itEditor = editorToProperty.constBegin();
    while (itEditor != editorToProperty.constEnd())
    {
        if (itEditor.key() == object)
        {
            QWidget *editor = itEditor.key();
            QtProperty *property = itEditor.value();
            editorToProperty.remove(editor);
            createdEditors[property].removeAll(editor);
            if (createdEditors[property].isEmpty())
                createdEditors.remove(property);
            return;
        }
        itEditor++;
    }
}

} // namespace PhotoLayoutsEditor
