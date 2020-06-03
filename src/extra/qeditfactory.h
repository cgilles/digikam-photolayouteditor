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

#ifndef QEDIT_FACTORY_H
#define QEDIT_FACTORY_H

// Qt includes

#include <QMap>
#include <QList>

// Local includes

#include "qteditorfactory.h"
#include "qtvariantproperty.h"
#include "qtpropertybrowser.h"

namespace PhotoLayoutsEditor
{

class QSpinBoxFactory : public QtSpinBoxFactory
{
    Q_OBJECT

public:

    QSpinBoxFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtIntPropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QSliderFactory : public QtSliderFactory
{
    Q_OBJECT

public:

    QSliderFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtIntPropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QScrollBarFactory : public QtScrollBarFactory
{
    Q_OBJECT

public:

    QScrollBarFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtIntPropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QCheckBoxFactory : public QtCheckBoxFactory
{
    Q_OBJECT

public:

    QCheckBoxFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtBoolPropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QDoubleSpinBoxFactory : public QtDoubleSpinBoxFactory
{
    Q_OBJECT

public:

    QDoubleSpinBoxFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtDoublePropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QLineEditFactory : public QtLineEditFactory
{
    Q_OBJECT

public:

    QLineEditFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtStringPropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QDateEditFactory : public QtDateEditFactory
{
    Q_OBJECT

public:

    QDateEditFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtDatePropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QTimeEditFactory : public QtTimeEditFactory
{
    Q_OBJECT

public:

    QTimeEditFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtTimePropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QDateTimeEditFactory : public QtDateTimeEditFactory
{
    Q_OBJECT

public:

    QDateTimeEditFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtDateTimePropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QKeySequenceEditorFactory : public QtKeySequenceEditorFactory
{
    Q_OBJECT

public:

    QKeySequenceEditorFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtKeySequencePropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QCharEditorFactory : public QtCharEditorFactory
{
    Q_OBJECT

public:

    QCharEditorFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtCharPropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QEnumEditorFactory : public QtEnumEditorFactory
{
    Q_OBJECT

public:

    QEnumEditorFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtEnumPropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QCursorEditorFactory : public QtCursorEditorFactory
{
    Q_OBJECT

public:

    QCursorEditorFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtCursorPropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QColorEditorFactory : public QtColorEditorFactory
{
    Q_OBJECT

public:

    QColorEditorFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtColorPropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QFontEditorFactory : public QtFontEditorFactory
{
    Q_OBJECT

public:

    QFontEditorFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtFontPropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QVariantEditorFactory : public QtVariantEditorFactory
{
    Q_OBJECT

public:

    QVariantEditorFactory(QObject* const parent = nullptr);

protected:

    QWidget* createEditor(QtVariantPropertyManager* manager, QtProperty* property, QWidget* parent);
};

// ---------------------------------------------------------------------

class QSliderEditFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT

public:

    explicit QSliderEditFactory(QObject* const parent = nullptr);

protected:

    virtual void connectPropertyManager(QtIntPropertyManager* manager);
    virtual QWidget* createEditor(QtIntPropertyManager* manager, QtProperty* property, QWidget* parent);
    virtual void disconnectPropertyManager(QtIntPropertyManager* manager);

Q_SIGNALS:

    void editingFinished();

private Q_SLOTS:

    void slotEditorDestroyed(QObject* const object);

private:

    QtSliderFactory*                    originalFactory;
    QMap<QtProperty*, QList<QWidget*> > createdEditors;
    QMap<QWidget*, QtProperty*>         editorToProperty;
};

} // namespace PhotoLayoutsEditor

#endif // QEDIT_FACTORY_H
