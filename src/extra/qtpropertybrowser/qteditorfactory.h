/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of a Qt Solutions component.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

#ifndef QTEDITORFACTORY_H
#define QTEDITORFACTORY_H

#include "qtpropertymanager.h"

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

class QtSpinBoxFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtSpinBoxFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT
public:
    explicit QtSpinBoxFactory(QObject* parent = nullptr);
    ~QtSpinBoxFactory();
protected:
    void connectPropertyManager(QtIntPropertyManager* manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtIntPropertyManager* manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtIntPropertyManager* manager) override;
private:
    QtSpinBoxFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtSpinBoxFactory)
    Q_DISABLE_COPY(QtSpinBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtSliderFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtSliderFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT
public:
    explicit QtSliderFactory(QObject* parent = nullptr);
    ~QtSliderFactory();
protected:
    void connectPropertyManager(QtIntPropertyManager* manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtIntPropertyManager* manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtIntPropertyManager* manager) override;
private:
    QtSliderFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtSliderFactory)
    Q_DISABLE_COPY(QtSliderFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtScrollBarFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtScrollBarFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT
public:
    explicit QtScrollBarFactory(QObject* parent = nullptr);
    ~QtScrollBarFactory();
protected:
    void connectPropertyManager(QtIntPropertyManager* manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtIntPropertyManager* manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtIntPropertyManager* manager) override;
private:
    QtScrollBarFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtScrollBarFactory)
    Q_DISABLE_COPY(QtScrollBarFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtCheckBoxFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtCheckBoxFactory : public QtAbstractEditorFactory<QtBoolPropertyManager>
{
    Q_OBJECT
public:
    explicit QtCheckBoxFactory(QObject* parent = nullptr);
    ~QtCheckBoxFactory();
protected:
    void connectPropertyManager(QtBoolPropertyManager *manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtBoolPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtBoolPropertyManager *manager) override;
private:
    QtCheckBoxFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtCheckBoxFactory)
    Q_DISABLE_COPY(QtCheckBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(bool))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtDoubleSpinBoxFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtDoubleSpinBoxFactory : public QtAbstractEditorFactory<QtDoublePropertyManager>
{
    Q_OBJECT
public:
    explicit QtDoubleSpinBoxFactory(QObject* parent = nullptr);
    ~QtDoubleSpinBoxFactory();
protected:
    void connectPropertyManager(QtDoublePropertyManager* manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtDoublePropertyManager* manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtDoublePropertyManager* manager) override;
private:
    QtDoubleSpinBoxFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtDoubleSpinBoxFactory)
    Q_DISABLE_COPY(QtDoubleSpinBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, double, double))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotDecimalsChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(double))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtLineEditFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtLineEditFactory : public QtAbstractEditorFactory<QtStringPropertyManager>
{
    Q_OBJECT
public:
    explicit QtLineEditFactory(QObject* parent = nullptr);
    ~QtLineEditFactory();
protected:
    void connectPropertyManager(QtStringPropertyManager *manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtStringPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtStringPropertyManager *manager) override;
private:
    QtLineEditFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtLineEditFactory)
    Q_DISABLE_COPY(QtLineEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QString &))
    Q_PRIVATE_SLOT(d_func(), void slotRegExpChanged(QtProperty *, const QRegExp &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QString &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtDateEditFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtDateEditFactory : public QtAbstractEditorFactory<QtDatePropertyManager>
{
    Q_OBJECT
public:
    explicit QtDateEditFactory(QObject* parent = nullptr);
    ~QtDateEditFactory();
protected:
    void connectPropertyManager(QtDatePropertyManager *manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtDatePropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtDatePropertyManager *manager) override;
private:
    QtDateEditFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtDateEditFactory)
    Q_DISABLE_COPY(QtDateEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QDate &))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *,
                        const QDate &, const QDate &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QDate &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtTimeEditFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtTimeEditFactory : public QtAbstractEditorFactory<QtTimePropertyManager>
{
    Q_OBJECT
public:
    explicit QtTimeEditFactory(QObject* parent = nullptr);
    ~QtTimeEditFactory();
protected:
    void connectPropertyManager(QtTimePropertyManager *manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtTimePropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtTimePropertyManager *manager) override;
private:
    QtTimeEditFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtTimeEditFactory)
    Q_DISABLE_COPY(QtTimeEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QTime &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QTime &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtDateTimeEditFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtDateTimeEditFactory : public QtAbstractEditorFactory<QtDateTimePropertyManager>
{
    Q_OBJECT
public:
    explicit QtDateTimeEditFactory(QObject* parent = nullptr);
    ~QtDateTimeEditFactory();
protected:
    void connectPropertyManager(QtDateTimePropertyManager *manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtDateTimePropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtDateTimePropertyManager *manager) override;
private:
    QtDateTimeEditFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtDateTimeEditFactory)
    Q_DISABLE_COPY(QtDateTimeEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QDateTime &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QDateTime &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtKeySequenceEditorFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtKeySequenceEditorFactory : public QtAbstractEditorFactory<QtKeySequencePropertyManager>
{
    Q_OBJECT
public:
    explicit QtKeySequenceEditorFactory(QObject* parent = nullptr);
    ~QtKeySequenceEditorFactory();
protected:
    void connectPropertyManager(QtKeySequencePropertyManager *manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtKeySequencePropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtKeySequencePropertyManager *manager) override;
private:
    QtKeySequenceEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtKeySequenceEditorFactory)
    Q_DISABLE_COPY(QtKeySequenceEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QKeySequence &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QKeySequence &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtCharEditorFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtCharEditorFactory : public QtAbstractEditorFactory<QtCharPropertyManager>
{
    Q_OBJECT
public:
    explicit QtCharEditorFactory(QObject* parent = nullptr);
    ~QtCharEditorFactory();
protected:
    void connectPropertyManager(QtCharPropertyManager *manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtCharPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtCharPropertyManager *manager) override;
private:
    QtCharEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtCharEditorFactory)
    Q_DISABLE_COPY(QtCharEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QChar &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QChar &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtEnumEditorFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtEnumEditorFactory : public QtAbstractEditorFactory<QtEnumPropertyManager>
{
    Q_OBJECT
public:
    explicit QtEnumEditorFactory(QObject* parent = nullptr);
    ~QtEnumEditorFactory();
protected:
    void connectPropertyManager(QtEnumPropertyManager* manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtEnumPropertyManager* manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtEnumPropertyManager* manager) override;
private:
    QtEnumEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtEnumEditorFactory)
    Q_DISABLE_COPY(QtEnumEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotEnumNamesChanged(QtProperty *,
                        const QStringList &))
    Q_PRIVATE_SLOT(d_func(), void slotEnumIconsChanged(QtProperty *,
                        const QMap<int, QIcon> &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtCursorEditorFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtCursorEditorFactory : public QtAbstractEditorFactory<QtCursorPropertyManager>
{
    Q_OBJECT
public:
    explicit QtCursorEditorFactory(QObject* parent = nullptr);
    ~QtCursorEditorFactory();
protected:
    void connectPropertyManager(QtCursorPropertyManager *manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtCursorPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtCursorPropertyManager *manager) override;
private:
    QtCursorEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtCursorEditorFactory)
    Q_DISABLE_COPY(QtCursorEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QCursor &))
    Q_PRIVATE_SLOT(d_func(), void slotEnumChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtColorEditorFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtColorEditorFactory : public QtAbstractEditorFactory<QtColorPropertyManager>
{
    Q_OBJECT
public:
    explicit QtColorEditorFactory(QObject* parent = nullptr);
    ~QtColorEditorFactory();
protected:
    void connectPropertyManager(QtColorPropertyManager *manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtColorPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtColorPropertyManager *manager) override;
private:
    QtColorEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtColorEditorFactory)
    Q_DISABLE_COPY(QtColorEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QColor &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QColor &))
};

class QtFontEditorFactoryPrivate;

class QT_QTPROPERTYBROWSER_EXPORT QtFontEditorFactory : public QtAbstractEditorFactory<QtFontPropertyManager>
{
    Q_OBJECT
public:
    explicit QtFontEditorFactory(QObject* parent = nullptr);
    ~QtFontEditorFactory();
protected:
    void connectPropertyManager(QtFontPropertyManager *manager) override;
    using QtAbstractEditorFactory::createEditor;
    QWidget *createEditor(QtFontPropertyManager *manager, QtProperty *property,
                QWidget *parent) override;
    void disconnectPropertyManager(QtFontPropertyManager *manager) override;
private:
    QtFontEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtFontEditorFactory)
    Q_DISABLE_COPY(QtFontEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QFont &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QFont &))
};

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

#endif
