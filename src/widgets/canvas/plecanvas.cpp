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

#include "plecanvas_p.h"

// Qt includes

#include <QPrinter>
#include <QPaintDevice>
#include <QPaintEngine>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>
#include <QSettings>

// Local includes

#include "plescene.h"
#include "layersmodel.h"
#include "layersmodelitem.h"
#include "layersselectionmodel.h"
#include "undomoverowscommand.h"
#include "imageloadingthread.h"
#include "pleglobal.h"
#include "progressevent.h"
#include "plewindow.h"
#include "plecanvasloadingthread.h"
#include "plecanvassavingthread.h"
#include "plestatusbar.h"

#define MAX_SCALE_LIMIT 4
#define MIN_SCALE_LIMIT 0.5

namespace PhotoLayoutsEditor
{

PLECanvas::PLECanvas(const PLECanvasSize& size, QWidget* parent)
    : QGraphicsView(parent),
      d(new PLECanvasPrivate)
{
    d->m_size = size;
    m_scene   = new PLEScene(QRectF(QPointF(0, 0), d->m_size.size(PLECanvasSize::Pixels)), this);
    this->init();
}

PLECanvas::PLECanvas(PLEScene* scene, QWidget* parent)
    : QGraphicsView(parent),
      d(new PLECanvasPrivate)
{
    Q_ASSERT(scene != nullptr);
    m_scene = scene;
    m_scene->setParent(this);
    this->setScene(m_scene);

    this->init();
}

PLECanvas::~PLECanvas()
{
    delete d;
}

void PLECanvas::init()
{
    m_is_saved       = true;
    m_saved_on_index = 0;
    m_undo_stack     = new QUndoStack(this);
    m_scale_factor   = 1;

    this->setupGUI();
    this->enableViewingMode();
    this->prepareSignalsConnection();
}

void PLECanvas::setupGUI()
{
    this->setAcceptDrops(true);
    this->setAutoFillBackground(true);
    this->viewport()->setAutoFillBackground(true);
    this->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    this->setCacheMode(QGraphicsView::CacheNone);

    QSettings config(QLatin1String("PhotoLayoutEditor"));
    config.beginGroup(QLatin1String("View"));
    this->setAntialiasing(config.value(QLatin1String("Antialiasing"), false).toBool());
    config.endGroup();

    // Transparent scene background
//    QPixmap pixmap(20,20);
//    QPainter p(&pixmap);
//    p.fillRect(0,0,20,20,Qt::lightGray);
//    p.fillRect(0,10,10,10,Qt::darkGray);
//    p.fillRect(10,0,10,10,Qt::darkGray);
//    QPalette viewportPalette = this->viewport()->palette();
//    viewportPalette.setBrush(QPalette::Background, QBrush(pixmap));
//    this->viewport()->setPalette(viewportPalette);
//    this->viewport()->setBackgroundRole(QPalette::Background);

    QVBoxLayout* const layout = new QVBoxLayout();
    this->setLayout(layout);
    layout->addWidget(this->viewport());

    this->setScene(m_scene);
}

void PLECanvas::prepareSignalsConnection()
{
    connect(m_scene, SIGNAL(selectionChanged()),
            this, SLOT(selectionChanged()));

    connect(m_scene, SIGNAL(itemAboutToBeRemoved(AbstractPhoto*)),
            this, SLOT(removeItem(AbstractPhoto*)));

    connect(m_scene, SIGNAL(itemsAboutToBeRemoved(QList<AbstractPhoto*>)),
            this, SLOT(removeItems(QList<AbstractPhoto*>)));

    connect(m_scene->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(selectionChanged(QItemSelection,QItemSelection)));

    connect(m_undo_stack, SIGNAL(indexChanged(int)),
            this, SLOT(isSavedChanged(int)));

    connect(m_undo_stack, SIGNAL(cleanChanged(bool)),
            this, SLOT(isSavedChanged(bool)));
}

void PLECanvas::setSelectionMode(SelectionMode mode)
{
    if      (mode & Viewing)
    {
        this->setInteractive(true);
        this->setDragMode(QGraphicsView::ScrollHandDrag);
        m_scene->setSelectionMode(PLEScene::NoSelection);
        goto save;
    }
    else if (mode & Zooming)
    {
        this->setInteractive(true);
        this->setDragMode(QGraphicsView::NoDrag);
        m_scene->setSelectionMode(PLEScene::NoSelection);
        goto save;
    }
    else if (mode & MultiSelecting)
    {
        this->setInteractive(true);
        this->setDragMode(QGraphicsView::RubberBandDrag);
        m_scene->setSelectionMode(PLEScene::MultiSelection);
        goto save;
    }
    else if (mode & SingleSelcting)
    {
        this->setInteractive(true);
        this->setDragMode(QGraphicsView::NoDrag);
        m_scene->setSelectionMode(PLEScene::SingleSelection);
        goto save;
    }

    return;

    save:

        m_selection_mode = mode;
}

LayersModel* PLECanvas::model() const
{
    return m_scene->model();
}

LayersSelectionModel* PLECanvas::selectionModel() const
{
    return m_scene->selectionModel();
}

PLECanvasSize PLECanvas::canvasSize() const
{
    return d->m_size;
}

void PLECanvas::setPLECanvasSize(const PLECanvasSize& size)
{
    if (!size.isValid())
        return;

    d->m_size = size;
    m_scene->setSceneRect(QRectF(QPointF(0,0), size.size(PLECanvasSize::Pixels)));
}

void PLECanvas::preparePrinter(QPrinter* printer)
{
    printer->setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
    PLECanvasSize::SizeUnits su = d->m_size.sizeUnit();
    QSizeF cs                   = d->m_size.size();
    bool setResolution          = true;

    switch (su)
    {
        case PLECanvasSize::Centimeters:
            cs *= 10;
            break;

        case PLECanvasSize::Milimeters:
            printer->setPaperSize(cs, QPrinter::Millimeter);
            break;

        case PLECanvasSize::Inches:
            printer->setPaperSize(cs, QPrinter::Inch);
            break;

        case PLECanvasSize::Points:
            printer->setPaperSize(cs, QPrinter::Point);
            break;

        case PLECanvasSize::Picas:
            printer->setPaperSize(cs, QPrinter::Pica);
            break;

        case PLECanvasSize::Pixels:
            printer->setPaperSize(cs, QPrinter::DevicePixel);
            setResolution = false;
            break;

        default:
            printer->setPaperSize(cs, QPrinter::DevicePixel);
            setResolution = false;
            qDebug() << "Unhandled size unit at:" << __FILE__ << ":" << __LINE__;
            break;
    }

    if (setResolution)
    {
        QSize printerResolution = d->m_size.resolution(PLECanvasSize::PixelsPerInch).toSize();
        printer->setResolution(qMin(printerResolution.width(), printerResolution.height()));
    }
}

void PLECanvas::addImage(const QImage& image)
{
    // Create & setup item
    PhotoItem* it = new PhotoItem(image);

    // Add item to scene & model
    m_scene->addItem(it);

    // Fits item to the scenes rect
    it->fitToRect(m_scene->sceneRect().toRect());
}

void PLECanvas::addImage(const QUrl& imageUrl)
{
    ImageLoadingThread* ilt = new ImageLoadingThread(this);
    ilt->setImageUrl(imageUrl);
    ilt->setMaximumProgress(0.9);

    connect(ilt, SIGNAL(imageLoaded(QUrl,QImage)),
            this, SLOT(imageLoaded(QUrl,QImage)));

    ilt->start();
}

void PLECanvas::addImages(const QList<QUrl> & images)
{
    ImageLoadingThread* ilt = new ImageLoadingThread(this);
    ilt->setImagesUrls(images);
    ilt->setMaximumProgress(0.9);

    connect(ilt, SIGNAL(imageLoaded(QUrl,QImage)),
            this, SLOT(imageLoaded(QUrl,QImage)));

    ilt->start();
}

void PLECanvas::addText(const QString& text)
{
    // Create & setup item
    TextItem* it = new TextItem(text);

    // Add item to scene & model
    m_scene->addItem(it);
}

void PLECanvas::addNewItem(AbstractPhoto* item)
{
    if (!item)
        return;

    m_scene->addItem(item);

    m_scene->clearSelection();
    m_scene->clearFocus();

    item->setSelected(true);
    item->setFocus( Qt::OtherFocusReason );
}

void PLECanvas::setAntialiasing(bool antialiasing)
{
    this->setRenderHint(QPainter::Antialiasing, antialiasing);                            /// It causes worst quality!
    this->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, antialiasing);    /// It causes worst quality!
    this->update();
}

void PLECanvas::imageLoaded(const QUrl& url, const QImage& image)
{
    if (!image.isNull())
    {
        // Create & setup item
        PhotoItem* it = new PhotoItem(image, url.fileName(), m_scene);
        // Add item to scene & model
        m_scene->addItem(it);
    }
}

void PLECanvas::moveRowsCommand(const QModelIndex& startIndex, int count, const QModelIndex& parentIndex, int move, const QModelIndex& destinationParent)
{
    int destination = startIndex.row();

    if (move > 0)
        destination += count + move;
    else if (move < 0)
        destination += move;
    else
        return;

    UndoMoveRowsCommand* undo = new UndoMoveRowsCommand(startIndex.row(), count, parentIndex, destination, destinationParent, model());
    m_undo_stack->push(undo);
}

void PLECanvas::moveSelectedRowsUp()
{
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

    if (!selectedIndexes.count())
    {
        qDebug() << "No items selected to move!" << selectedIndexes;
        return;
    }

    // Check continuity of selection
    QModelIndexList::iterator it = selectedIndexes.begin();
    QModelIndex startIndex = *it;

    if (startIndex.isValid())
    {
        int minRow = startIndex.row();
        int maxRow = startIndex.row();
        int sumRows = startIndex.row();

        for (++it; it != selectedIndexes.end(); ++it)
        {
            if (it->column() != LayersModelItem::NameString)
                continue;

            if      (startIndex.parent() != it->parent())
            {
                qDebug() << "Different parents of items!\n" << selectedIndexes;
                return;
            }
            else if (!it->isValid())
            {
                qDebug() << "Invalid items!\n" << selectedIndexes;
                return;
            }

            if (it->row() < minRow)
            {
                startIndex = *it;
                minRow = it->row();
            }

            if (it->row() > maxRow)
                maxRow = it->row();

            sumRows += it->row();
        }

        if ((((minRow+maxRow)*(maxRow-minRow+1))/2.0) != sumRows)
        {
            qDebug() << "Unordered items!\n" << selectedIndexes;
            return;
        }

        if (minRow) // It means "is there any space before starting index to move selection"
            moveRowsCommand(startIndex, selectedIndexes.count(), startIndex.parent(), -1, startIndex.parent());
    }

    this->selectionChanged();
}

void PLECanvas::moveSelectedRowsDown()
{
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

    if (!selectedIndexes.count())
    {
        qDebug() << "No items selected to move!" << selectedIndexes;
        return;
    }

    // Check continuity of selection
    QModelIndexList::iterator it = selectedIndexes.begin();
    QModelIndex startIndex = *it;

    if (startIndex.isValid())
    {
        int minRow = startIndex.row();
        int maxRow = startIndex.row();
        int sumRows = startIndex.row();

        for (++it; it != selectedIndexes.end(); ++it)
        {
            if (it->column() != LayersModelItem::NameString)
                continue;

            if      (startIndex.parent() != it->parent())
            {
                qDebug() << "Different parents of items!\n" << selectedIndexes;
                return;
            }
            else if (!it->isValid())
            {
                qDebug() << "Invalid items!\n" << selectedIndexes;
                return;
            }

            if (it->row() < minRow)
            {
                startIndex = *it;
                minRow = it->row();
            }

            if (it->row() > maxRow)
                maxRow = it->row();

            sumRows += it->row();
        }

        if ((((minRow+maxRow)*(maxRow-minRow+1))/2.0) != sumRows)
        {
            qDebug() << "Unordered items!\n" << selectedIndexes;
            return;
        }

        if (maxRow+1 < model()->rowCount(startIndex.parent())) // It means "is there any space before starting index to move selection"
            moveRowsCommand(startIndex, selectedIndexes.count(), startIndex.parent(), 1, startIndex.parent());
    }

    this->selectionChanged();
}

void PLECanvas::removeItem(AbstractPhoto* item)
{
    if (item)
        m_scene->removeItem(item);
}

void PLECanvas::removeItems(const QList<AbstractPhoto*>& items)
{
    m_scene->removeItems(items);
}

void PLECanvas::removeSelectedRows()
{
    QList<AbstractPhoto*> items;
    QModelIndexList selectedIndexes = selectionModel()->selectedRows();

    foreach (const QModelIndex& index, selectedIndexes)
        items << static_cast<LayersModelItem*>(index.internalPointer())->photo();

    m_scene->removeItems(items);
}

void PLECanvas::selectionChanged()
{
    QList<AbstractPhoto*> selectedItems = m_scene->selectedItems();
    QModelIndexList oldSelected = selectionModel()->selectedIndexes();
    QModelIndexList newSelected = model()->itemsToIndexes(selectedItems);

    foreach (const QModelIndex& index, oldSelected)
    {
        if (!newSelected.contains(index) && index.column() == LayersModelItem::NameString)
            selectionModel()->select(index, QItemSelectionModel::Rows | QItemSelectionModel::Deselect);
    }

    foreach (const QModelIndex& index, newSelected)
    {
        if (!selectionModel()->isSelected(index) && index.column() == LayersModelItem::NameString)
            selectionModel()->select(index, QItemSelectionModel::Rows | QItemSelectionModel::Select);
    }

    // Selection change signals

    selectedItems = m_scene->selectedItems();

    if (m_selection_mode & SingleSelcting)
    {
        if (selectedItems.count() == 1)
        {
            AbstractPhoto* item = selectedItems.at(0);
            Q_EMIT hasSelectionChanged(true);
            Q_EMIT selectedItem(item);
        }
        else
        {
            Q_EMIT hasSelectionChanged(false);
            Q_EMIT selectedItem(nullptr);
        }
    }
    else if (m_selection_mode & MultiSelecting)
    {
        Q_EMIT hasSelectionChanged(selectedItems.count());
    }
}

void PLECanvas::selectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection)
{
    LayersModelItem* temp;
    const QModelIndexList& oldSel = oldSelection.indexes();
    const QModelIndexList& newSel = newSelection.indexes();
    QSet<QModelIndex> deselected = oldSel.toSet().subtract(newSel.toSet());

    foreach (const QModelIndex& index, deselected)
    {
        if (index.column() != LayersModelItem::NameString)
            continue;

        temp = static_cast<LayersModelItem*>(index.internalPointer());

        if (temp->photo() && temp->photo()->isSelected())
            temp->photo()->setSelected(false);
    }

    QSet<QModelIndex> selected = newSel.toSet().subtract(oldSel.toSet());

    foreach (const QModelIndex& index, selected)
    {
        if (index.column() != LayersModelItem::NameString)
            continue;

        temp = static_cast<LayersModelItem*>(index.internalPointer());

        if (temp->photo() && !temp->photo()->isSelected())
            temp->photo()->setSelected(true);
    }
}

void PLECanvas::enableDefaultSelectionMode()
{
    this->unsetCursor();
    m_scene->setInteractionMode(PLEScene::Selecting | PLEScene::Moving);
    setSelectionMode(MultiSelecting);
    m_scene->clearSelectingFilters();
}

void PLECanvas::enableViewingMode()
{
    this->unsetCursor();
    m_scene->setInteractionMode(PLEScene::View);
    setSelectionMode(Viewing);
    m_scene->clearSelectingFilters();
}

void PLECanvas::enableZoomingMode()
{
    this->unsetCursor();
    setSelectionMode(Zooming);
    this->setCursor(QCursor(QPixmap(QLatin1String(":/zoom_cursor.png")).scaled(QSize(24,24))));
    m_scene->clearSelectingFilters();
}

void PLECanvas::enablePLECanvasEditingMode()
{
    this->unsetCursor();
    m_scene->setInteractionMode(PLEScene::NoSelection);
    setSelectionMode(Viewing);
    this->setCursor(QCursor(Qt::ArrowCursor));
    m_scene->clearSelectingFilters();
}

void PLECanvas::enableEffectsEditingMode()
{
    this->unsetCursor();
    m_scene->setInteractionMode(PLEScene::Selecting);
    setSelectionMode(SingleSelcting);
    this->setCursor(QCursor(Qt::ArrowCursor));
    m_scene->clearSelectingFilters();
    m_scene->addSelectingFilter(PhotoItem::staticMetaObject);
}

void PLECanvas::enableTextEditingMode()
{
    this->unsetCursor();
    m_scene->setInteractionMode(PLEScene::Selecting | PLEScene::MouseTracking | PLEScene::OneclickFocusItems);
    setSelectionMode(SingleSelcting);
    m_scene->clearSelectingFilters();
    m_scene->addSelectingFilter(TextItem::staticMetaObject);
}

void PLECanvas::enableRotateEditingMode()
{
    this->unsetCursor();
    m_scene->setInteractionMode(PLEScene::Selecting | PLEScene::Rotating);
    setSelectionMode(SingleSelcting);
    this->setCursor(Qt::ArrowCursor);
    m_scene->clearSelectingFilters();
}

void PLECanvas::enableScaleEditingMode()
{
    this->unsetCursor();
    m_scene->setInteractionMode(PLEScene::Selecting | PLEScene::Scaling);
    setSelectionMode(SingleSelcting);
    this->setCursor(Qt::ArrowCursor);
    m_scene->clearSelectingFilters();
}

void PLECanvas::enableCropEditingMode()
{
    this->unsetCursor();
    m_scene->setInteractionMode(PLEScene::Selecting | PLEScene::Cropping);
    setSelectionMode(SingleSelcting);
    this->setCursor(Qt::ArrowCursor);
    m_scene->clearSelectingFilters();
}

void PLECanvas::enableBordersEditingMode()
{
    this->unsetCursor();
    m_scene->setInteractionMode(PLEScene::Selecting);
    setSelectionMode(SingleSelcting);
    this->setCursor(Qt::ArrowCursor);
    m_scene->clearSelectingFilters();
}

void PLECanvas::refreshWidgetConnections(bool isVisible)
{
    if (isVisible)
    {
        connect(this, SIGNAL(hasSelectionChanged(bool)),
                sender(), SLOT(setEnabled(bool)));

        Q_EMIT hasSelectionChanged(m_scene->selectedItems().count());
    }
    else
    {
        disconnect(this, SIGNAL(hasSelectionChanged(bool)),
                   sender(), nullptr);
    }
}

void PLECanvas::newUndoCommand(QUndoCommand* command)
{
    m_undo_stack->push(command);
}

void PLECanvas::progressEvent(ProgressEvent* event)
{
    QProgressBar* temp = d->progressMap[event->sender()];

    switch (event->type())
    {
        case ProgressEvent::Init:

            if (!temp)
                this->layout()->addWidget( temp = d->progressMap[event->sender()] = new QProgressBar(this) );

            temp->setMaximum(1000);
            temp->setValue(0);
            this->setEnabled(false);

            {
                PLEStatusBar* sb = dynamic_cast<PLEStatusBar*>(PLEWindow::instance()->statusBar());

                if (sb)
                    sb->runBusyIndicator();
            }

            break;

        case ProgressEvent::ProgressUpdate:

            if (temp)
                temp->setValue((int)(event->data().toDouble() * 1000.));

            break;

        case ProgressEvent::ActionUpdate:

            if (temp)
                temp->setFormat(event->data().toString() + QLatin1String(" [%p%]"));

            break;

        case ProgressEvent::Finish:

            if (temp)
            {
                temp->setValue( temp->maximum() );
                d->progressMap.take(event->sender())->deleteLater();
            }

            this->setEnabled(true);

            {
                PLEStatusBar* sb = dynamic_cast<PLEStatusBar*>(PLEWindow::instance()->statusBar());

                if (sb)
                    sb->stopBusyIndicator();
            }

            break;

        default:

            temp = nullptr;
            break;
    }

    event->setAccepted(temp);
}

void PLECanvas::wheelEvent(QWheelEvent* event)
{
    int steps = event->delta() / 120;
    double scaleFactor;
    scaleFactor = (m_scale_factor + steps * 0.1) / m_scale_factor;
    scale(scaleFactor, event->pos());
}

QDomDocument PLECanvas::toSvg() const
{
    QDomDocument result(QLatin1String(" svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\""));
    QDomElement svg;// = ;//m_scene->toSvg(result);
    result.appendChild(svg);
    svg.setAttribute(QLatin1String("width"), QString::number(d->m_size.size().width()));
    svg.setAttribute(QLatin1String("height"), QString::number(d->m_size.size().height()));

    switch (d->m_size.sizeUnit())
    {
        case PLECanvasSize::Centimeters:
            svg.setAttribute(QLatin1String("width"), svg.attribute(QLatin1String("width")) + QLatin1String("cm"));
            svg.setAttribute(QLatin1String("height"), svg.attribute(QLatin1String("height")) + QLatin1String("cm"));
            break;

        case PLECanvasSize::Milimeters:
            svg.setAttribute(QLatin1String("width"), svg.attribute(QLatin1String("width")) + QLatin1String("mm"));
            svg.setAttribute(QLatin1String("height"), svg.attribute(QLatin1String("height")) + QLatin1String("mm"));
            break;

        case PLECanvasSize::Inches:
            svg.setAttribute(QLatin1String("width"), svg.attribute(QLatin1String("width")) + QLatin1String("in"));
            svg.setAttribute(QLatin1String("height"), svg.attribute(QLatin1String("height")) + QLatin1String("in"));
            break;

        case PLECanvasSize::Picas:
            svg.setAttribute(QLatin1String("width"), svg.attribute(QLatin1String("width")) + QLatin1String("pc"));
            svg.setAttribute(QLatin1String("height"), svg.attribute(QLatin1String("height")) + QLatin1String("pc"));
            break;

        case PLECanvasSize::Points:
            svg.setAttribute(QLatin1String("width"), svg.attribute(QLatin1String("width")) + QLatin1String("pt"));
            svg.setAttribute(QLatin1String("height"), svg.attribute(QLatin1String("height")) + QLatin1String("pt"));
            break;

        case PLECanvasSize::Pixels:
            svg.setAttribute(QLatin1String("width"), svg.attribute(QLatin1String("width")) + QLatin1String("px"));
            svg.setAttribute(QLatin1String("height"), svg.attribute(QLatin1String("height")) + QLatin1String("px"));
            break;

        default:
            svg.setAttribute(QLatin1String("width"), svg.attribute(QLatin1String("width")) + QLatin1String("px"));
            svg.setAttribute(QLatin1String("height"), svg.attribute(QLatin1String("height")) + QLatin1String("px"));
            qDebug() << "Unhandled size unit at:" << __FILE__ << ":" << __LINE__;
    }

    QDomElement resolution = result.createElementNS(PhotoLayoutsEditor::uri(), QLatin1String("page"));
    resolution.setAttribute(QLatin1String("width"), QString::number(d->m_size.resolution().width()));
    resolution.setAttribute(QLatin1String("height"), QString::number(d->m_size.resolution().height()));
    resolution.setAttribute(QLatin1String("unit"), PLECanvasSize::resolutionUnitName(d->m_size.resolutionUnit()));
    svg.appendChild(resolution);

    return result;
}

PLECanvas* PLECanvas::fromSvg(QDomDocument& document)
{
    PLECanvas* result     = nullptr;
    QDomNodeList children = document.childNodes();

    if (children.count())
    {
        QDomElement element = document.firstChildElement(QLatin1String("svg"));

        if (!element.isNull())
        {
            QString width = element.attribute(QLatin1String("width"));
            QString height = element.attribute(QLatin1String("height"));
            QDomElement pageElement = element.firstChildElement(QLatin1String("page"));
            QString xResolution = pageElement.attribute(QLatin1String("width"));
            QString yResolution = pageElement.attribute(QLatin1String("height"));
            QString resUnit = pageElement.attribute(QLatin1String("unit"));
            qDebug() << pageElement.namespaceURI() << PhotoLayoutsEditor::templateUri();

            // PLECanvas size validation
            QRegExp sizeRegExp(QLatin1String("[0-9.]+((cm)|(mm)|(in)|(pc)|(pt)|(px))"));
            QRegExp resRegExp(QLatin1String("[0-9.]+"));

            if (sizeRegExp.exactMatch(width) &&
                sizeRegExp.exactMatch(height) &&
                width.right(2) == height.right(2) &&
                resRegExp.exactMatch(xResolution) &&
                resRegExp.exactMatch(yResolution) &&
                PLECanvasSize::resolutionUnit(resUnit) != PLECanvasSize::UnknownResolutionUnit)
            {
                PLECanvasSize size;
                size.setSizeUnit( PLECanvasSize::sizeUnit(width.right(2)) );
                width.remove(width.length()-2, 2);
                height.remove(height.length()-2, 2);
                QSizeF dimension(width.toDouble(),height.toDouble());
                size.setSize(dimension);
                size.setResolutionUnit( PLECanvasSize::resolutionUnit(resUnit) );
                QSizeF resolution(xResolution.toDouble(), yResolution.toDouble());
                size.setResolution(resolution);

                if (dimension.isValid())
                {
                    QDomElement sceneElement = element.firstChildElement(QLatin1String("g"));

                    while (!sceneElement.isNull() && sceneElement.attribute(QLatin1String("id")) != QLatin1String("PLEScene"))
                        sceneElement = sceneElement.nextSiblingElement(QLatin1String("g"));

                    PLEScene* scene = PLEScene::fromSvg(sceneElement);

                    if (scene)
                    {
                        result = new PLECanvas(scene);
                        result->setEnabled(false);
                        result->d->m_size = size;
                        result->d->m_template = (pageElement.namespaceURI() == PhotoLayoutsEditor::templateUri());
                    }
                }
            }
            else
            {
                QMessageBox::critical(qApp->activeWindow(), QObject::tr("Error"), QObject::tr("Invalid image size!"));
            }
        }
    }

    return result;
}

void PLECanvas::scale(qreal factor, const QPoint& center)
{
    // Scaling limitation
    if (factor <= 0 || !scene() || ((m_scale_factor*factor <= 0.1 && factor < 1) || (m_scale_factor*factor > 7)))
        return;

    QGraphicsView::scale(factor, factor);

    if (center.isNull())
        centerOn( m_scene->sceneRect().center() );
    else
        centerOn( mapToScene(center) );

    m_scale_factor *= factor;
}

void PLECanvas::scale(const QRect& rect)
{
    QRectF r(this->mapToScene(rect.topLeft()),
             this->mapToScene(rect.bottomRight()));
    QSizeF viewSize = r.size();
    viewSize.setWidth(  qAbs(viewSize.width())  );
    viewSize.setHeight( qAbs(viewSize.height()) );
    QSizeF sceneSize = m_scene->sceneRect().size();
    qreal xFactor = sceneSize.width() / viewSize.width();
    qreal yFactor = sceneSize.height() / viewSize.height();
    qreal newFactor;

    if (xFactor > 1 && yFactor > 1)
        newFactor = xFactor > yFactor ? xFactor : yFactor;
    else
        newFactor = xFactor < yFactor ? xFactor : yFactor;

    if (m_scale_factor*newFactor > 7)
        newFactor = 7 / m_scale_factor;

    this->scale(newFactor, rect.center());
}

QUrl PLECanvas::file() const
{
    return m_file;
}

void PLECanvas::setFile(const QUrl& file)
{
    if (file.isValid() && !file.isEmpty())
        m_file = file;
}

void PLECanvas::save(const QUrl& fileUrl, bool setAsDefault)
{
    QUrl tempFile = fileUrl;

    if (fileUrl.isEmpty() || !fileUrl.isValid())
    {
        if (m_file.isEmpty() || !m_file.isValid())
        {
            QMessageBox::critical(qApp->activeWindow(),
                                  QObject::tr("Can't save canvas!"),
                                  QObject::tr("Invalid file path."));
            return;
        }

        tempFile = m_file;
    }

    if (setAsDefault)
       m_file = tempFile;

    PLECanvasSavingThread* thread = new PLECanvasSavingThread(this);

    connect(thread, SIGNAL(saved()),
            this, SLOT(savingFinished()));

    thread->save(this, m_file);
}

void PLECanvas::saveTemplate(const QUrl& fileUrl)
{
    if (fileUrl.isEmpty() || !fileUrl.isValid())
    {
        QMessageBox::critical(qApp->activeWindow(),
                              QObject::tr("Can't save canvas!"),
                              QObject::tr("Invalid file path."));
        return;
    }

    PLECanvasSavingThread* thread = new PLECanvasSavingThread(this);

    connect(thread, SIGNAL(saved()),
            this, SLOT(savingFinished()));

    thread->saveAsTemplate(this, fileUrl);
}

bool PLECanvas::isSaved()
{
    return m_is_saved;
}

void PLECanvas::isSavedChanged(int /*currentCommandIndex*/)
{
    m_is_saved = (m_saved_on_index == m_undo_stack->index());

    Q_EMIT savedStateChanged();
}

void PLECanvas::isSavedChanged(bool /*isStackClean*/)
{
    if (m_undo_stack->isClean())
        m_is_saved = m_undo_stack->isClean();
    else
        m_is_saved = (m_saved_on_index == m_undo_stack->index());

    Q_EMIT savedStateChanged();
}

bool PLECanvas::isTemplate() const
{
    return d->m_template;
}

void PLECanvas::savingFinished()
{
    m_is_saved       = true;
    m_saved_on_index = m_undo_stack->index();

    Q_EMIT savedStateChanged();
}

void PLECanvas::renderPLECanvas(QPaintDevice* device)
{
    if (scene())
    {
        scene()->setSelectionVisible(false);
        bool isGridVisible = scene()->isGridVisible();
        scene()->setGridVisible(false);

        scene()->setSelectionVisible(false);
        QPainter p(device);

        if (d->m_size.sizeUnit() != PLECanvasSize::Pixels &&
            d->m_size.sizeUnit() != PLECanvasSize::UnknownSizeUnit)
        {
            QSizeF resolution = d->m_size.resolution(PLECanvasSize::PixelsPerInch);
            p.setTransform( QTransform::fromScale( device->logicalDpiX() / resolution.width(),
                                                   device->logicalDpiY() / resolution.height()) );
        }

        scene()->render(&p, scene()->sceneRect(), scene()->sceneRect());
        p.end();

        scene()->setSelectionVisible(true);
        scene()->setGridVisible(isGridVisible);
    }
}

void PLECanvas::renderPLECanvas(QPrinter* device)
{
    renderPLECanvas(static_cast<QPaintDevice*>(device));
}

void PLECanvas::beginRowsRemoving()
{
    m_undo_stack->beginMacro(QObject::tr("Remove items"));
}

void PLECanvas::endRowsRemoving()
{
    m_undo_stack->endMacro();
}

} // namespace PhotoLayoutsEditor

#undef MAX_SCALE_LIMIT
#undef MIN_SCALE_LIMIT
