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

#include "plescene.h"

// C++ std includes

#include <limits>

// Qt includes

#include <QGraphicsTextItem>
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMenu>
#include <QGraphicsSceneDragDropEvent>
#include <QStyleOptionGraphicsItem>
#include <QMap>
#include <QGraphicsWidget>
#include <qmath.h>
#include <QUndoCommand>
#include <QImageReader>
#include <QtAlgorithms>
#include <QBuffer>
#include <QUrl>
#include <QApplication>
#include <QMimeData>
#include <QTemporaryFile>
#include <QSettings>

// Local includes

#include "pleglobal.h"
#include "rotationwidgetitem.h"
#include "scalingwidgetitem.h"
#include "cropwidgetitem.h"
#include "plescenebackground.h"
#include "mousepresslistener.h"
#include "toolsdockwidget.h"
#include "plewindow.h"
#include "imageloadingthread.h"
#include "progressevent.h"
#include "plecanvasloadingthread.h"
#include "photoitem.h"
#include "plesceneborder.h"
#include "imagedialog.h"
#include "layersmodel.h"
#include "layersmodelitem.h"
#include "layersselectionmodel.h"

using namespace Digikam;

namespace PhotoLayoutsEditor
{

QColor PLEScene::OUTSIDE_SCENE_COLOR;

class PLEScenePrivate
{
    explicit PLEScenePrivate(PLEScene* scene)
      : m_scene(scene),
        model(new LayersModel(scene)),
        selection_model(new LayersSelectionModel(model, scene)),
        m_pressed_object(nullptr),
        m_pressed_item(nullptr),
        m_selected_items_all_movable(true),
        m_selection_visible(true),
        m_rot_item(nullptr),
        m_scale_item(nullptr),
        m_crop_item(nullptr),
//        m_blend_active(false),
        m_readPLESceneMousePress_listener(nullptr),
        m_readPLESceneMousePress_enabled(false),
        m_hovered_photo(nullptr)
    {
        // Background of the scene
        m_background = new PLESceneBackground(m_scene);
        // Border of the scene
        m_border = new PLESceneBorder(m_scene);
    }

    QList<QGraphicsItem*> itemsAtPosition(const QPointF& scenePos, QWidget* widget)
    {
        QGraphicsView* view = widget ? qobject_cast<QGraphicsView*>(widget->parentWidget()) : nullptr;

        if (!view)
            return m_scene->items(scenePos, Qt::IntersectsItemShape, Qt::DescendingOrder, QTransform());

        const QRectF pointRect(scenePos, QSizeF(1, 1));

        if (!view->isTransformed())
            return m_scene->items(pointRect, Qt::IntersectsItemShape, Qt::DescendingOrder);

        const QTransform viewTransform = view->viewportTransform();

        return m_scene->items(pointRect, Qt::IntersectsItemShape, Qt::DescendingOrder, viewTransform);
    }

    AbstractItemInterface* itemAt(const QPointF& scenePos, QWidget* widget)
    {
        QList<QGraphicsItem*> items = itemsAtPosition(scenePos, widget);

        return items.count() ? dynamic_cast<AbstractItemInterface*>(items.first()) : nullptr;
    }

    QList<AbstractItemInterface*> itemsAt(const QPointF& scenePos, QWidget* widget)
    {
        QList<QGraphicsItem*> items = itemsAtPosition(scenePos, widget);
        QList<AbstractItemInterface*> r;

        foreach (QGraphicsItem* const i, items)
        {
            AbstractItemInterface* iface = dynamic_cast<AbstractItemInterface*>(i);

            if (iface)
                r.push_back(iface);
        }

        return r;
    }

    void sendPressEventToItem(AbstractItemInterface* item, QGraphicsSceneMouseEvent* event)
    {
        if (!item)
            return;

        // Send mousepressevent to the pressed item
        event->setPos(item->mapFromScene(event->scenePos()));
        event->setButtonDownPos(event->button(),
                                item->mapFromScene(event->buttonDownScenePos(event->button())));
        event->setLastPos(item->mapFromScene(event->lastScenePos()));
        item->mousePressEvent(event);
    }

    void sendMoveEventToItem(AbstractItemInterface* item, QGraphicsSceneMouseEvent* event)
    {
        if (!item)
            return;

        // Send mousepressevent to the pressed item
        event->setPos(item->mapFromScene(event->scenePos()));
        event->setButtonDownPos(event->button(),
                                item->mapFromScene(event->buttonDownScenePos(event->button())));
        event->setLastPos(item->mapFromScene(event->lastScenePos()));
        item->mouseMoveEvent(event);
    }

    void sendReleaseEventToItem(AbstractItemInterface* item, QGraphicsSceneMouseEvent* event)
    {
        if (!item)
            return;

        // Send mousepressevent to the pressed item
        event->setPos(item->mapFromScene(event->scenePos()));
        event->setButtonDownPos(event->button(),
                                item->mapFromScene(event->buttonDownScenePos(event->button())));
        event->setLastPos(item->mapFromScene(event->lastScenePos()));
        item->mouseReleaseEvent(event);
    }

    void sendDoubleClickEventToItem(AbstractItemInterface* item, QGraphicsSceneMouseEvent* event)
    {
        if (!item)
            return;

        // Send mousepressevent to the pressed item
        event->setPos(item->mapFromScene(event->scenePos()));
        event->setButtonDownPos(event->button(),
                                item->mapFromScene(event->buttonDownScenePos(event->button())));
        event->setLastPos(item->mapFromScene(event->lastScenePos()));
        item->mouseDoubleClickEvent(event);
    }

    // Used for selecting items
    void deselectSelected()
    {
        m_selected_items_all_movable = true;

        foreach (AbstractItemInterface* const photo, m_selected_items.keys())
        {
            photo->setSelected(false);

            if (photo->hasFocus())
                photo->clearFocus();
        }

        m_selected_items.clear();
        m_selected_items_path = QPainterPath();
    }

    bool selectPressed()
    {
        if (m_pressed_item)
        {
            // Select if not selested

            if (!m_pressed_item->isSelected())
            {
                m_selected_items.insert(m_pressed_item, m_pressed_item->pos());
                m_selected_items_path        = m_selected_items_path.united(m_pressed_item->mapToScene(m_pressed_item->shape()));
                m_selected_items_all_movable = ((m_pressed_item->flags() & QGraphicsItem::ItemIsMovable) != 0) && m_selected_items_all_movable;
                m_pressed_item->setSelected(true);
                setSelectionInitialPosition();
            }

            return true;
        }

        return false;
    }

    void focusPressed()
    {
        if (!m_pressed_item)
            return;

        // If is selectable and focusable try to set focus and post mousepressevent to it

        if (m_pressed_item->flags() & QGraphicsItem::ItemIsFocusable)
            m_pressed_item->setFocus(Qt::MouseFocusReason);
    }

    void setSelectionInitialPosition()
    {
        QMap<AbstractPhoto*,QPointF>::iterator it = m_selected_items.begin();

        while (it != m_selected_items.end())
        {
            it.value() = it.key()->pos();
            ++it;
        }

        m_selected_items_path_initial_pos = m_selected_items_path.boundingRect().topLeft();
    }

    bool wasMoved()
    {
        QMap<AbstractPhoto*,QPointF>::iterator it = m_selected_items.begin();

        while (it != m_selected_items.end())
        {
            if (it.value() != it.key()->pos())
                return true;
            ++it;
        }

        return false;
    }

    // Parent scene
    QGraphicsScene*              m_scene;
    // PLEScene's model
    LayersModel*                 model;
    LayersSelectionModel*        selection_model;
    // Background item
    PLESceneBackground*          m_background;
    // Border item
    PLESceneBorder*              m_border;

    QMap<AbstractPhoto*,QPointF> m_selected_items;
    AbstractItemInterface*       m_pressed_object;
    AbstractPhoto*               m_pressed_item;
    QPainterPath                 m_selected_items_path;
    QPointF                      m_selected_items_path_initial_pos;
    bool                         m_selected_items_all_movable;
    bool                         m_selection_visible;
    QList<const char*>           m_selection_filters;
    QPointF                      paste_scene_pos;

    // Used for rotating items
    RotationWidgetItem*          m_rot_item;

    // Used for scaling item
    ScalingWidgetItem*           m_scale_item;

    // Used for cropping items
    CropWidgetItem*              m_crop_item;
//    bool                         m_blend_active;

    // For reading mouse press
    MousePressListener*          m_readPLESceneMousePress_listener;
    bool                         m_readPLESceneMousePress_enabled;

    // Used for drag&drop images
    PhotoItem*                   m_hovered_photo;

private:

    PLEScenePrivate(const PLEScenePrivate&) = delete;
    PLEScenePrivate& operator=(const PLEScenePrivate&) = delete;

    friend class PLEScene;
};

class AddItemsCommand
    : public QUndoCommand
{
    QList<AbstractPhoto*> items;
    int                   position;
    PLEScene*             scene;
    bool                  done;

public:

    AddItemsCommand(AbstractPhoto* item, int position, PLEScene* scene, QUndoCommand* parent = nullptr)
        : QUndoCommand(QObject::tr("Add item"), parent),
          position(position),
          scene(scene),
          done(false)
    {
        items << item;
    }

    AddItemsCommand(const QList<AbstractPhoto*>& items, int position, PLEScene* scene, QUndoCommand* parent = nullptr)
        : QUndoCommand(QObject::tr("Add item", "Add items", items.count()), parent),
          items(items),
          position(position),
          scene(scene),
          done(false)
    {
    }

    ~AddItemsCommand() override
    {
        if (done)
            return;

        foreach (AbstractPhoto* const item, items)
            item->deleteLater();

        items.clear();
    }

    void redo() override
    {
        foreach (AbstractPhoto* const item, items)
            scene->QGraphicsScene::addItem(item);

        scene->model()->insertItems(items, position);
        done = true;
    }

    void undo() override
    {
        QRectF region;

        foreach (AbstractPhoto* const item, items)
        {
            region = region.united( item->mapRectToScene(item->boundingRect()) );

            if (item->isSelected())
                item->setSelected(false);

            scene->QGraphicsScene::removeItem(item);
        }

        scene->model()->removeRows(position, items.count());
        scene->update(region);
        done = false;
    }
};

class MoveItemsCommand
    : public QUndoCommand
{
    QMap<AbstractPhoto*, QPointF> m_items;
    PLEScene*                     m_scene;
    bool                          done;

public:

    MoveItemsCommand(const QMap<AbstractPhoto*, QPointF>& items, PLEScene* scene, QUndoCommand* parent = nullptr)
        : QUndoCommand(QObject::tr("Move item", "Move items", items.count()), parent),
          m_items(items),
          m_scene(scene),
          done(true)
    {
    }

    void redo() override
    {
        if (!done)
        {
            QMap<AbstractPhoto*, QPointF>::iterator it = m_items.begin();

            while (it != m_items.end())
            {
                QPointF temp = it.key()->pos();
                it.key()->setPos( it.value() );
                it.value() = temp;
                ++it;
            }

            done = !done;
            m_scene->calcSelectionBoundingRect();
        }
    }

    void undo() override
    {
        if (done)
        {
            QMap<AbstractPhoto*, QPointF>::iterator it = m_items.begin();

            while (it != m_items.end())
            {
                QPointF temp = it.key()->pos();
                it.key()->setPos( it.value() );
                it.value() = temp;
                ++it;
            }

            done = !done;
            m_scene->calcSelectionBoundingRect();
        }
    }
};

class RemoveItemsCommand
    : public QUndoCommand
{
    AbstractPhoto* m_item;
    int            item_row;
    AbstractPhoto* item_parent;
    PLEScene*      m_scene;
    bool           done;

public:

    RemoveItemsCommand(AbstractPhoto* item, PLEScene* scene, QUndoCommand* parent = nullptr)
        : QUndoCommand(QLatin1String("Remove item"), parent),
            m_item(item),
            item_row(0),
            m_scene(scene),
            done(false)
    {
        item_parent = dynamic_cast<AbstractPhoto*>(item->parentItem());
    }

    ~RemoveItemsCommand() override
    {
        if (done)
        {
            if (m_item && !m_item->scene() && !m_item->parentItem())
                delete m_item;
        }
    }

    void redo() override
    {
        QPersistentModelIndex parentIndex = QPersistentModelIndex(m_scene->model()->findIndex( item_parent ));

        if (item_parent && !(parentIndex.isValid() && item_parent->scene()))
            return;

        // Remove from model
        QModelIndex itemIndex = m_scene->model()->findIndex(m_item, parentIndex);
        item_row = itemIndex.row();

        if (itemIndex.isValid())
            m_scene->model()->removeRow(item_row, parentIndex);

        // Remove from scene
        if (m_item->scene() == m_scene)
            m_scene->QGraphicsScene::removeItem(m_item);

        done = true;
    }

    void undo() override
    {
        if (!done)
            return;

        // Add to scene
        if (m_item->scene() != m_scene)
            m_scene->QGraphicsScene::addItem( m_item );

        m_item->setParentItem( item_parent );

        // Add to model
        QPersistentModelIndex parentIndex = QPersistentModelIndex( m_scene->model()->findIndex( item_parent ) );

        if (!m_scene->model()->hasIndex(item_row, 0, parentIndex) ||
            static_cast<LayersModelItem*>(m_scene->model()->index(item_row, 0, parentIndex).internalPointer())->photo() != m_item)
        {
            if (m_scene->model()->insertRow(item_row, parentIndex))
            {
                static_cast<LayersModelItem*>(m_scene->model()->index(item_row, 0, parentIndex).internalPointer())->setPhoto(m_item);
                // Add items children to model
                appendChild(m_item, m_scene->model()->index(item_row, 0, parentIndex));
            }
        }

        done = false;
    }

private:

    static bool compareGraphicsItems(QGraphicsItem* i1, QGraphicsItem* i2)
    {
        if ((i1 && i2) && (i1->zValue() < i2->zValue()))
            return true;

        return false;
    }

    void appendChild(AbstractPhoto* item, const QModelIndex& parent)
    {
        QList<QGraphicsItem*> items = item->childItems();

        if (items.count())
        {
            // Sort using z-Values (z-Value == models row)
            std::sort(items.begin(), items.end(), PhotoLayoutsEditor::RemoveItemsCommand::compareGraphicsItems);
            int i = 0;

            foreach (QGraphicsItem* const childItem, items)
            {
                AbstractPhoto* const photo = dynamic_cast<AbstractPhoto*>(childItem);

                if (photo)
                {
                    if (m_scene->model()->insertRow(i,parent))
                    {
                        static_cast<LayersModelItem*>(m_scene->model()->index(i, 0, parent).internalPointer())->setPhoto(photo);
                        this->appendChild(photo, m_scene->model()->index(i, 0, parent));
                        ++i;
                    }
                }
            }
        }
    }
};

class CropItemsCommand
    : public QUndoCommand
{
    QMap<AbstractPhoto*,QPainterPath> data;

public:

    CropItemsCommand(const QPainterPath& path, const QList<AbstractPhoto*>& items, QUndoCommand* parent = nullptr)
        : QUndoCommand(QObject::tr("Crop item", "Crop items", items.count()), parent)
    {
        qDebug() << "scene crop shape" << path.boundingRect();

        foreach (AbstractPhoto* const item, items)
            data.insert(item, item->mapFromScene(path));
    }

    void redo() override
    {
        this->run();
    }

    void undo() override
    {
        this->run();
    }

private:

    void run()
    {
        for (QMap<AbstractPhoto*,QPainterPath>::iterator it = data.begin(); it != data.end(); ++it)
        {
            QPainterPath temp = it.key()->cropShape();
            it.key()->setCropShape( it.value() );
            it.value() = temp;
        }
    }
};

PLEScene::PLEScene(const QRectF& dimension, QObject* const parent)
    : QGraphicsScene(dimension, parent),
      d(new PLEScenePrivate(this)),
      x_grid(0),
      y_grid(0),
      grid_item(nullptr),
      grid_changed(true)
{
    if (!OUTSIDE_SCENE_COLOR.isValid())
    {
        QPalette pal = this->palette();
        OUTSIDE_SCENE_COLOR = pal.color(QPalette::Window);
        OUTSIDE_SCENE_COLOR.setAlpha(190);
    }

    this->setBackgroundBrush(Qt::transparent);

    // Mouse interaction mode
    setInteractionMode(DEFAULT_EDITING_MODE);

    // Create default grid

    QSettings config(QLatin1String("PhotoLayoutEditor"));
    config.beginGroup(QLatin1String("View"));
    setGrid(config.value(QLatin1String("XGrid"), 25.0).toDouble(), config.value(QLatin1String("YGrid"), 25.0).toDouble());
    grid_visible = !config.value(QLatin1String("ShowGrid"), false).toBool();
    setGridVisible(config.value(QLatin1String("ShowGrid"), false).toBool());
    config.endGroup();

    // Indexing method
    this->setItemIndexMethod(QGraphicsScene::NoIndex);

    // Signal connections
    connect(this, SIGNAL(selectionChanged()),
            this, SLOT(updateSelection()));
}

PLEScene::~PLEScene()
{
    delete d;
}

PLESceneBackground* PLEScene::background()
{
    return d->m_background;
}

PLESceneBorder* PLEScene::border()
{
    return d->m_border;
}

LayersModel* PLEScene::model() const
{
    return d->model;
}

LayersSelectionModel* PLEScene::selectionModel() const
{
    return d->selection_model;
}

void PLEScene::addItem(AbstractPhoto* item)
{
    // Prevent multiple addition of the item

    if (item->scene() == this)
        return;

    QModelIndexList selectedIndexes = d->selection_model->selectedIndexes();
    int insertionRow = -1;

    foreach (const QModelIndex& index, selectedIndexes)
    {
        if (index.column() != LayersModelItem::NameString)
            continue;
        if (insertionRow > index.row())
            insertionRow = index.row();
    }

    if (insertionRow < 0)
        insertionRow = 0;

    QUndoCommand* command = new AddItemsCommand(item, insertionRow, this);
    PLE_PostUndoCommand(command);
}

void PLEScene::addItems(const QList<AbstractPhoto*>& items)
{
    // Prevent multiple addition of the item
    QList<AbstractPhoto*> tempItems;

    foreach (AbstractPhoto* const item, items)
    {
        if (item->scene() == this && this->model()->findIndex(item).isValid())
            continue;

        tempItems.prepend(item);
    }

    if (tempItems.count() < 1)
        return;

    QModelIndexList selectedIndexes = d->selection_model->selectedIndexes();
    int insertionRow = -1;

    foreach (const QModelIndex& index, selectedIndexes)
    {
        if (index.column() != LayersModelItem::NameString)
            continue;

        if (insertionRow > index.row())
            insertionRow = index.row();
    }

    if (insertionRow < 0)
        insertionRow = 0;

    QUndoCommand* parent = nullptr;
    QUndoCommand* command = nullptr;

    if (items.count() > 1)
        parent = new QUndoCommand( QObject::tr("Add item", "Add items", items.count()) );

    foreach (AbstractPhoto* const item, tempItems)
        command = new AddItemsCommand(item, insertionRow++, this, parent);

    if      (parent)
        PLE_PostUndoCommand(parent);
    else if (command)
        PLE_PostUndoCommand(command);
}

void PLEScene::removeItem(AbstractPhoto* item)
{
    if (!askAboutRemoving(1))
        return;

    QUndoCommand* command = new RemoveItemsCommand(item, this);
    PLE_PostUndoCommand(command);
}

void PLEScene::removeItems(const QList<AbstractPhoto*>& items)
{
    if (!askAboutRemoving(items.count()))
    {
        return;
    }

    QUndoCommand* command = nullptr;
    QUndoCommand* parent  = nullptr;

    if (items.count() > 1)
    {
        parent = new QUndoCommand( QObject::tr("Remove item", "Remove items", items.count()) );
        PLE_PostUndoCommand(parent);
    }

    foreach (AbstractPhoto* const item, items)
    {
        command = new RemoveItemsCommand(item, this, parent);
        PLE_PostUndoCommand(command);
    }
}

void PLEScene::removeSelectedItems()
{
    removeItems(selectedItems());
}

void PLEScene::changeSelectedImage()
{
    QList<AbstractPhoto*> items = selectedItems();

    if (items.count() != 1)
        return;

    PhotoItem* item = dynamic_cast<PhotoItem*>(items.first());

    if (!item)
        return;

    QUrl url = ImageDialog::getImageURL(PLEWindow::instance(), QUrl());

    if (url.isEmpty())
        return;

    ImageLoadingThread* ilt = new ImageLoadingThread(this);
    ilt->setImageUrl(url);
    ilt->setMaximumProgress(1);
    connect(ilt, SIGNAL(imageLoaded(QUrl,QImage)), item, SLOT(imageLoaded(QUrl,QImage)));
    ilt->start();
}

void PLEScene::contextMenuEvent(QGraphicsSceneMouseEvent* event)
{
    QMenu menu;

    // Remove items
    QList<AbstractPhoto*> items = this->selectedItems();

    if (items.count())
    {
        if (items.count() == 1)
        {
            PhotoItem* item = dynamic_cast<PhotoItem*>(items.first());

            if (item)
            {
                QAction* removeAction = menu.addAction( QObject::tr("Change item's image") );
                connect(removeAction, SIGNAL(triggered()), this, SLOT(changeSelectedImage()));
            }
        }

        QAction* removeAction = menu.addAction( QObject::tr("Delete selected item", "Delete selected items", items.count()) );
        connect(removeAction, SIGNAL(triggered()), this, SLOT(removeSelectedItems()));
        menu.addSeparator();
    }

    // Background
    QAction* background = menu.addAction( QObject::tr("Canvas background") );

    connect(background, SIGNAL(triggered()),
            ToolsDockWidget::instance(), SLOT(setPLECanvasWidgetVisible()));

    menu.exec(event->screenPos());
}


void PLEScene::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    this->contextMenuEvent( (QGraphicsSceneMouseEvent*) event );
}

void PLEScene::keyPressEvent(QKeyEvent* event)
{
    if (this->focusItem())
    {
        QGraphicsScene::keyPressEvent(event);
        event->setAccepted(true);
        return;
    }

    switch (event->key())
    {
        case Qt::Key_Delete:
            this->removeItems(selectedItems());
            event->setAccepted(true);
            break;

        case Qt::Key_Escape:
            //disableitemsDrawing();
            break;
    }

    if (event->isAccepted())
        return;

    QGraphicsScene::keyPressEvent(event);
}

void PLEScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        // Return mouse position to the listener
        if (d->m_readPLESceneMousePress_enabled)
        {
            d->m_readPLESceneMousePress_listener->mousePress(event->scenePos());
            event->setAccepted(true);
            return;
        }

        // If moving enabled
        if (m_interaction_mode & Selecting)
        {
            this->calcSelectionBoundingRect();

            // Get initial selection position
            d->setSelectionInitialPosition();

            // If single selection mode, clear CTRL modifier
            if (m_selectionMode & SingleSelection)
                event->setModifiers(QFlags<Qt::KeyboardModifier>(((int)event->modifiers()) & ~Qt::ControlModifier));

            // Get items under mouse
            d->m_pressed_object = d->m_pressed_item = nullptr;
            QList<AbstractItemInterface*> itemsList = d->itemsAt(event->scenePos(), event->widget());

            foreach (AbstractItemInterface* const i, itemsList)
            {
                // Get pressed item
                d->m_pressed_object = i;
                // Test if this is a photo/text item
                d->m_pressed_item = dynamic_cast<AbstractPhoto*>(d->m_pressed_object);

                if ((m_interaction_mode & Rotating) && d->m_pressed_object == d->m_rot_item)
                {
                    // If it is rotation widget
                    d->sendPressEventToItem(d->m_pressed_object, event);

                    if (event->isAccepted())
                        return;
                }
                else if ((m_interaction_mode & Scaling) && d->m_pressed_object == d->m_scale_item)
                {
                    // If it is scaling widget
                    d->sendPressEventToItem(d->m_pressed_object, event);

                    if (event->isAccepted())
                        return;
                }
                else if ((m_interaction_mode & Cropping) && d->m_pressed_object == d->m_crop_item)
                {
                    // If it is cropping widget
                    d->sendPressEventToItem(d->m_pressed_object, event);

                    if (event->isAccepted())
                        return;
                }
                else
                {
                    break;
                }
            }

            // If event pos is not in current selection shape...

            if (!d->m_selected_items_path.contains(event->scenePos()) || !d->m_selected_items.contains(dynamic_cast<AbstractPhoto*>(d->m_pressed_item)))
            {
                // Clear focus from focused items
                if (this->focusItem())
                    this->focusItem()->clearFocus();

                // Clear this selection
                if (!(event->modifiers() & Qt::ControlModifier))
                    d->deselectSelected();
            }

            // Filtering selection

            if (d->m_pressed_item &&
                d->m_selection_filters.count() &&
                !d->m_selection_filters.contains( d->m_pressed_item->metaObject()->className() ))
            {
                d->m_pressed_item = nullptr;
            }

            // If there is VALID item to select...

            if      (d->m_pressed_item)
            {
                // If not selectable -> deselect item

                if (!(d->m_pressed_item->flags() & QGraphicsItem::ItemIsSelectable))
                    d->m_pressed_item = nullptr;
                else
                    d->sendPressEventToItem(d->m_pressed_item, event);
            }
            else if (m_interaction_mode & MouseTracking)
            {
                // If listeners should know scene press position
                Q_EMIT mousePressedPoint(event->buttonDownScenePos(event->button()));
            }

            setRotationWidgetVisible(m_interaction_mode & Rotating);
            setScalingWidgetVisible(m_interaction_mode & Scaling);
            setCropWidgetVisible(m_interaction_mode & Cropping);
        }

        event->setAccepted(m_interaction_mode & Selecting);
    }
    else if (event->button() == Qt::RightButton)
    {
        this->contextMenuEvent(event);
    }
}

void PLEScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (d->m_readPLESceneMousePress_enabled)
            return;

        if (d->m_pressed_object)
            d->sendMoveEventToItem(d->m_pressed_object, event);

        if (m_interaction_mode & Moving && !event->isAccepted())
        {
            // Selecting pressed item

            event->setAccepted(d->selectPressed());

            // Moving items

            if (d->m_selected_items_all_movable)
            {
                // Calculate movement

                QPointF distance = event->scenePos() - event->buttonDownScenePos(Qt::LeftButton) + d->m_selected_items_path_initial_pos;

                if (event->modifiers() & Qt::ShiftModifier && this->isGridVisible())
                {
                    distance.setX(x_grid*round(distance.rx()/x_grid));
                    distance.setY(y_grid*round(distance.ry()/y_grid));
                }

                QPointF difference = d->m_selected_items_path.boundingRect().topLeft();
                d->m_selected_items_path.translate(distance);
                d->m_selected_items_path.translate(-difference);
                difference = distance - difference;
                d->m_selected_items_path.translate(difference);

                foreach (AbstractItemInterface* const item, d->m_selected_items.keys())
                    item->moveBy(difference.x(), difference.y());
            }
        }
    }
}


void PLEScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (d->m_readPLESceneMousePress_enabled)
        {
            d->m_readPLESceneMousePress_listener->mouseRelease(event->scenePos());
            event->setAccepted(true);
            return;
        }

        if (m_interaction_mode & Selecting)
        {
            // Selecting pressed item

            event->setAccepted(d->selectPressed());

            if (m_interaction_mode & OneclickFocusItems)
                d->focusPressed();

            // Send mousereleaseevent to the released item

            if (d->m_pressed_object)
                d->sendReleaseEventToItem(d->m_pressed_object, event);

            // Post move command to QUndoStack

            if ((m_interaction_mode & Moving) && d->wasMoved())
            {
                qDebug() << "move command from scene";
                QUndoCommand* command = new MoveItemsCommand(d->m_selected_items, this);
                PLE_PostUndoCommand(command);
            }
        }
    }
}

void PLEScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        // In this app there is no difference between doubleClick and press events.
        // in Qt mouse events are alled in this order:        pressEvent -> releaseEvent -> doubleClickEvent -> releaseEvent
        // So for correct working second call of releaseEvent it is needed to call mousePressEvent here.

        this->mousePressEvent(event);

        // If selecting enabled -> focus item

        if (m_interaction_mode & Selecting)
            d->focusPressed();
    }
}

void PLEScene::drawBackground(QPainter* painter, const QRectF& rect)
{
    // Transparent scene background

    if (isSelectionVisible())
    {
        QTransform tr = painter->transform().inverted();
        QPixmap pixmap(20,20);
        QPainter p(&pixmap);
        p.fillRect(0,0,20,20,Qt::lightGray);
        p.fillRect(0,10,10,10,Qt::darkGray);
        p.fillRect(10,0,10,10,Qt::darkGray);
        QBrush b(pixmap);
        b.setTransform(tr);
        painter->fillRect(rect, b);
    }

    // Fill scene outside sceneRect with semi-transparent window color
    {
        QPainterPath p;
        p.addRect(rect);
        QPainterPath s;
        s.addRect(this->sceneRect());
        painter->fillPath(p.subtracted(s), OUTSIDE_SCENE_COLOR);
    }
}

void PLEScene::drawForeground(QPainter* painter, const QRectF& rect)
{
    QGraphicsScene::drawForeground(painter, rect.intersected(this->sceneRect()));

    // Draw selected items shape

    if (isSelectionVisible())
    {
        this->calcSelectionBoundingRect();
        painter->save();
        painter->setPen(Qt::red);
        painter->setCompositionMode(QPainter::RasterOp_NotSourceAndNotDestination);
        painter->drawPath(d->m_selected_items_path);
        painter->restore();
    }
}

void PLEScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    if (canDecode(event->mimeData()))
    {
        event->setDropAction(Qt::CopyAction);
        event->setAccepted(true);
    }
    else
    {
        event->setDropAction(Qt::IgnoreAction);
        event->setAccepted(false);
    }
}

void PLEScene::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
    if (d->m_hovered_photo)
    {
        d->m_hovered_photo->dragLeaveEvent(event);
        d->m_hovered_photo = nullptr;
    }
}

void PLEScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    PhotoItem* tempItem = dynamic_cast<PhotoItem*>(this->itemAt(event->scenePos(), QTransform()));

    // Send event to item

    if (tempItem)
    {
        if (d->m_hovered_photo != tempItem)
        {
            if (tempItem)
                tempItem->dragEnterEvent(event);

            if (d->m_hovered_photo)
                d->m_hovered_photo->dragLeaveEvent(event);
        }
        else
        {
            tempItem->dragMoveEvent(event);
        }
    }
    else
    {
        // Proces event on scene

        if (d->m_hovered_photo)
            d->m_hovered_photo->dragLeaveEvent(event);

        if (canDecode(event->mimeData()))
        {
            event->setDropAction(Qt::CopyAction);
            event->setAccepted(true);
        }
        else
        {
            // Ignore event
            event->setDropAction(Qt::IgnoreAction);
            event->setAccepted(false);
        }
    }

    d->m_hovered_photo = tempItem;
}


void PLEScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    PhotoItem* item = dynamic_cast<PhotoItem*>(this->itemAt(event->scenePos(), QTransform()));

    if (item)
    {
        item->dropEvent(event);
        return;
    }

    d->paste_scene_pos = event->scenePos();

    const QMimeData* mimeData = event->mimeData();

    if ( PLEWindow::instance()->hasInterface() &&
         mimeData->hasFormat(QLatin1String("digikam/item-ids")))
    {
        QList<QUrl> urls;
        QByteArray ba = mimeData->data(QLatin1String("digikam/item-ids"));
        QDataStream ds(&ba, QIODevice::ReadOnly);
        ds >> urls;

        ImageLoadingThread* ilt = new ImageLoadingThread(this);
        ilt->setImagesUrls(urls);
        ilt->setMaximumProgress(0.9);

        connect(ilt, SIGNAL(imageLoaded(QUrl,QImage)),
                this, SLOT(imageLoaded(QUrl,QImage)));

        ilt->start();
    }
    else if (mimeData->hasFormat(QLatin1String("text/uri-list")))
    {
        QList<QUrl> urls = mimeData->urls();
        QList<QUrl> list;

        foreach (const QUrl& url, urls)
            list << QUrl(url);

        ImageLoadingThread* ilt = new ImageLoadingThread(this);
        ilt->setImagesUrls(list);
        ilt->setMaximumProgress(0.9);

        connect(ilt, SIGNAL(imageLoaded(QUrl,QImage)),
                this, SLOT(imageLoaded(QUrl,QImage)));

        ilt->start();
    }

    event->setAccepted( true );
}

void PLEScene::setGrid(double x, double y)
{
    // Grid can't be 0

    if (x == 0 || y == 0)
    {
        return;
    }

    this->x_grid = x;
    this->y_grid = y;

    if (!grid_visible)
    {
        return;
    }

    qDebug() << "Check to create new grid";

    if (!grid_item)
    {
        QList<QGraphicsItem*> items;
        grid_item = createItemGroup(items);
        grid_item->setZValue(0);
        grid_item->setVisible(true);
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
        effect->setOpacity(0.5);
        grid_item->setGraphicsEffect(effect);
    }

    qreal width                        = sceneRect().width();
    qreal height                       = sceneRect().height();
    QList<QGraphicsItem*> children     = grid_item->childItems();
    QList<QGraphicsItem*>::iterator it = children.begin();
    QGraphicsLineItem* temp1           = nullptr;

    qDebug() << "Apply horizontal grid";

    for (qreal i = x; i < width; i+=x)
    {
        if (it != children.end())
        {
            temp1 = dynamic_cast<QGraphicsLineItem*>(*it);
            temp1->setLine(i, 0, i, height);
            ++it;
        }
        else
        {
            temp1 = new QGraphicsLineItem(i, 0, i, height, nullptr);
            grid_item->addToGroup(temp1);
        }
    }

    qDebug() << "Apply Vertical grid";

    for (qreal i = y; i < height; i+=y)
    {
        if (it != children.end())
        {
            temp1 = dynamic_cast<QGraphicsLineItem*>(*it);
            temp1->setLine(0, i, width, i);
            ++it;
        }
        else
        {
            temp1 = new QGraphicsLineItem(0, i, width, i, nullptr);
            grid_item->addToGroup(temp1);
        }
    }

    QList<QGraphicsItem*> toRemove;

    while (it != children.end())
    {
        toRemove.append(*(it++));
    }

    while (toRemove.count())
    {
        QGraphicsItem* const temp2 = toRemove.takeAt(0);
        grid_item->removeFromGroup(temp2);
        delete temp2;
    }
}

void PLEScene::setHorizontalGrid(double x)
{
    setGrid(x, this->y_grid);
}

void PLEScene::setVerticalGrid(double y)
{
    this->setGrid(this->x_grid, y);
}

void PLEScene::setGridVisible(bool visible)
{
    if (grid_visible == visible)
        return;

    grid_visible = visible;

    if (visible)
    {
        this->setGrid(x_grid, y_grid);
    }
    else
    {
        delete grid_item;
        grid_item = nullptr;
    }
}

bool PLEScene::isGridVisible()
{
    return this->grid_visible;
}

void PLEScene::setInteractionMode(int mode)
{
    m_interaction_mode = mode;
    setRotationWidgetVisible(mode & Rotating);
    setScalingWidgetVisible(mode & Scaling);
    setCropWidgetVisible(mode & Cropping);
}

void PLEScene::setSelectionMode(SelectionMode selectionMode)
{
    switch (selectionMode)
    {
        case NoSelection:
            this->setSelectionArea(QPainterPath());
            m_selectionMode = selectionMode;
            break;

        case MultiSelection:
            m_selectionMode = selectionMode;
            break;

        case SingleSelection:
            this->setSelectionArea(QPainterPath());
            m_selectionMode = selectionMode;
            break;
    }
}

void PLEScene::addSelectingFilter(const QMetaObject & classMeta)
{
    d->m_selection_filters.push_back(classMeta.className());
}

void PLEScene::clearSelectingFilters()
{
    d->m_selection_filters.clear();
}

void PLEScene::setRotationWidgetVisible(bool isVisible)
{
    if (d->m_rot_item)
    {
        if (d->m_pressed_object == d->m_rot_item)
            d->m_pressed_object = nullptr;

        this->QGraphicsScene::removeItem(d->m_rot_item);
        d->m_rot_item->deleteLater();
        d->m_rot_item = nullptr;
    }

    if (isVisible && d->m_selected_items.count())
    {
        if (!d->m_rot_item)
            d->m_rot_item = new RotationWidgetItem(d->m_selected_items.keys());

        d->m_rot_item->setZValue(std::numeric_limits<double>::infinity());
        this->QGraphicsScene::addItem(d->m_rot_item);
    }
}

void PLEScene::setScalingWidgetVisible(bool isVisible)
{
    if (d->m_scale_item)
    {
        if (d->m_pressed_object == d->m_scale_item)
            d->m_pressed_object = nullptr;

        this->QGraphicsScene::removeItem(d->m_scale_item);
        d->m_scale_item->deleteLater();
        d->m_scale_item = nullptr;
    }

    if (isVisible && d->m_selected_items.count())
    {
        if (!d->m_scale_item)
            d->m_scale_item = new ScalingWidgetItem(d->m_selected_items.keys());

        d->m_scale_item->setZValue(std::numeric_limits<double>::infinity());
        this->QGraphicsScene::addItem(d->m_scale_item);
        this->update(d->m_scale_item->boundingRect());
    }
}

void PLEScene::setCropWidgetVisible(bool isVisible)
{
    if (d->m_crop_item)
    {
        if (d->m_pressed_object == d->m_crop_item)
            d->m_pressed_object = nullptr;

        this->QGraphicsScene::removeItem(d->m_crop_item);
        d->m_crop_item->deleteLater();
        d->m_crop_item = nullptr;
    }

    if (isVisible && d->m_selected_items.count())
    {
        if (!d->m_crop_item)
        {
            d->m_crop_item = new CropWidgetItem();
            connect(d->m_crop_item, SIGNAL(cancelCrop()), this, SLOT(closeCropWidget()));
        }

        d->m_crop_item->setZValue(std::numeric_limits<double>::infinity());
        this->QGraphicsScene::addItem(d->m_crop_item);

        if (dynamic_cast<PhotoLayoutsEditor::TextItem*>(d->m_pressed_item))
        {
            // It makes no sense to crop a text item
            d->m_crop_item->hide();
        }
        else if (d->m_selected_items.count() == 1)
        {
            d->m_crop_item->setItems(d->m_selected_items.keys());
        }
        else
        {
            d->m_crop_item->hide();
        }
    }
    else if (m_interaction_mode & Cropping)
    {
        this->clearSelection();
    }
}

void PLEScene::closeCropWidget()
{
    this->setCropWidgetVisible(false);
}

qreal PLEScene::gridHorizontalDistance() const
{
    return this->x_grid;
}

qreal PLEScene::gridVerticalDistance() const
{
    return this->y_grid;
}

QDomDocument PLEScene::toSvg(ProgressObserver* observer)
{
    return toSvg(observer, false);
}

QDomDocument PLEScene::toTemplateSvg(ProgressObserver* observer)
{
    return toSvg(observer, true);
}

QDomDocument PLEScene::toSvg(ProgressObserver* observer, bool asTemplate)
{
    QDomDocument document;

    QDomElement sceneElement = document.createElement(QLatin1String("g"));
    sceneElement.setAttribute(QLatin1String("id"), QLatin1String("PLEScene"));
    sceneElement.setAttribute(QLatin1String("width"), QString::number(this->width()));
    sceneElement.setAttribute(QLatin1String("height"), QString::number(this->height()));
    document.appendChild(sceneElement);

    if (asTemplate)
    {
        QDomElement previewImage = document.createElement(QLatin1String("defs"));
        previewImage.setAttribute(QLatin1String("id"), QLatin1String("Preview"));
        QDomElement image = document.createElement(QLatin1String("image"));

        QSizeF sceneSize = this->sceneRect().size();
        qreal imgw = 200, imgh = 200;

        if ((imgw / sceneSize.width()) < (imgh / sceneSize.height()))
            imgh = qRound(sceneSize.height() * imgw / sceneSize.width());
        else
            imgw = qRound(sceneSize.width() * imgh / sceneSize.height());

        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        QImage img(QSize((int)imgw, (int)imgh), QImage::Format_ARGB32_Premultiplied);
        img.fill(Qt::white);
        QPainter p(&img);
        this->render(&p, QRectF(0, 0, imgw, imgh), this->sceneRect(), Qt::KeepAspectRatio);
        p.end();
        QTemporaryFile temp;

        if (temp.open())
        {
            img.save(temp.fileName());
            img.save(&buffer, "PNG");
        }

        image.appendChild( document.createTextNode( QString::fromUtf8(byteArray.toBase64()) ) );
        image.setAttribute(QLatin1String("width"),QString::number((int)imgw));
        image.setAttribute(QLatin1String("height"),QString::number((int)imgh));

        previewImage.appendChild(image);
        sceneElement.appendChild(previewImage);
    }

    QList<QGraphicsItem*> itemsList = this->items(Qt::AscendingOrder);

    if (observer)
        observer->progresChanged(0);

    //--------------------------------------------------------

    if (observer)
        observer->progresName( QObject::tr("Saving background...") );

    QDomElement background = document.createElement(QLatin1String("g"));
    background.setAttribute(QLatin1String("class"), QLatin1String("background"));
    background.appendChild(d->m_background->toSvg(document));
    sceneElement.appendChild(background);

    if (observer)
        observer->progresChanged(1.0 / (double)(itemsList.count()+1.0));

    //--------------------------------------------------------

    int i = 1;
    foreach (QGraphicsItem* const item, itemsList)
    {
        AbstractPhoto* const photo = dynamic_cast<AbstractPhoto*>(item);

        if (photo)
        {
            if (observer)
                observer->progresName( QObject::tr("Saving %1...").arg(photo->name()) );

            QDomDocument photoItemDocument = asTemplate ? photo->toTemplateSvg() : photo->toSvg();
            sceneElement.appendChild( photoItemDocument.documentElement() );
        }

        if (observer)
            observer->progresChanged((double)i++ / (double)(itemsList.count()+1.0));
    }

    //--------------------------------------------------------

    if (observer)
        observer->progresName( QObject::tr("Saving border...") );

    QDomElement border = document.createElement(QLatin1String("g"));
    border.setAttribute(QLatin1String("class"), QLatin1String("border"));
    border.appendChild(d->m_border->toSvg(document));
    sceneElement.appendChild(border);

    if (observer)
        observer->progresChanged(1.0 / (double)(itemsList.count()+1.0));

    return document;
}

PLEScene* PLEScene::fromSvg(QDomElement& sceneElement)
{
    if (sceneElement.isNull() || sceneElement.tagName() != QLatin1String("g") || sceneElement.attribute(QLatin1String("id")) != QLatin1String("PLEScene"))
        return nullptr;

    // PLEScene dimension
    qreal xPLESceneRect = 0;
    qreal yPLESceneRect = 0;
    qreal widthPLESceneRect = sceneElement.attribute(QLatin1String("width")).toDouble();
    qreal heightPLESceneRect = sceneElement.attribute(QLatin1String("height")).toDouble();
    QRectF dimension(xPLESceneRect,yPLESceneRect,widthPLESceneRect,heightPLESceneRect);
    PLEScene* result = new PLEScene(dimension);

    // Loading thread
    PLECanvasLoadingThread * thread = new PLECanvasLoadingThread(result);

    // Create elements
    int errorsCount = 0;
    QDomNodeList children = sceneElement.childNodes();

    for (int i = 0; i < children.count(); ++i)
    {
        QDomElement element = children.at(i).toElement();

        if (element.isNull() || element.tagName() != QLatin1String("g"))
            continue;

        QString itemClass = element.attribute(QLatin1String("class"));
        AbstractPhoto* item;

        if      (itemClass == QLatin1String("PhotoItem"))
        {
            item = new PhotoItem();
            thread->addItem(item, element);
        }
        else if (itemClass == QLatin1String("TextItem"))
        {
            item = new TextItem();
            thread->addItem(item, element);
        }
        else if (itemClass == QLatin1String("background"))
        {
            thread->addBackground(result->d->m_background, element);
            continue;
        }
        else if (itemClass == QLatin1String("border"))
        {
            thread->addBorder(result->d->m_border, element);
            continue;
        }
        else
        {
            continue;
        }

        // If created add item to scene

        if (item)
        {
            result->QGraphicsScene::addItem(item);
            result->model()->insertItem(item, 0, result->model()->findIndex( dynamic_cast<AbstractPhoto*>(item->parentItem()) ));
            item->setZValue(i+1);
        }
        else
        {
            ++errorsCount;
        }
    }

    thread->start();

    // Show error message
    if (errorsCount)
    {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Unable to create one element", "Unable to create %1 elements", errorsCount));
    }

    return result;
}


void PLEScene::render(QPainter* painter, const QRectF& target, const QRectF& source, Qt::AspectRatioMode aspectRatioMode)
{
    if (d->m_rot_item)
        d->m_rot_item->hide();

    if (d->m_scale_item)
        d->m_scale_item->hide();

    d->m_selection_visible = false;

    QGraphicsScene::render(painter, target, source, aspectRatioMode);

    d->m_selection_visible = true;

    if (d->m_rot_item)
        d->m_rot_item->show();

    if (d->m_scale_item)
        d->m_scale_item->show();
}

void PLEScene::readPLESceneMousePress(MousePressListener * mouseListsner)
{
    d->m_readPLESceneMousePress_listener = mouseListsner;

    if (mouseListsner)
        d->m_readPLESceneMousePress_enabled = true;
    else
        d->m_readPLESceneMousePress_enabled = false;
}

QList<AbstractPhoto*> PLEScene::selectedItems() const
{
    QList<AbstractPhoto*> result;
    const QList<QGraphicsItem*> & list = QGraphicsScene::selectedItems();

    foreach (QGraphicsItem* const item, list)
        result << dynamic_cast<AbstractPhoto*>(item);

    return result;
}

void PLEScene::updateSelection()
{
    foreach (AbstractPhoto* const item, d->m_selected_items.keys())
        if (!item->isSelected())
            d->m_selected_items.remove(item);

    d->m_selected_items_path = QPainterPath();
    QList<AbstractPhoto*> itemsList = this->selectedItems();

    foreach (AbstractPhoto* const item, itemsList)
    {
        if (d->m_selection_filters.count() && !d->m_selection_filters.contains( item->metaObject()->className() ))
        {
            item->setSelected(false);
            d->m_selected_items.remove(item);
            continue;
        }

        if (!d->m_selected_items.contains(item))
            d->m_selected_items.insert(item, item->pos());

        d->m_selected_items_path = d->m_selected_items_path.united(item->mapToScene(item->shape()));
    }

    if (d->m_selected_items.count() == 1 && d->m_selected_items.begin().key()->flags() & QGraphicsItem::ItemIsFocusable)
        d->m_selected_items.begin().key()->setFocus(Qt::OtherFocusReason);

    this->setRotationWidgetVisible(m_interaction_mode & Rotating);
    this->setScalingWidgetVisible(m_interaction_mode & Scaling);
    this->setCropWidgetVisible(m_interaction_mode & Cropping);
}

void PLEScene::imageLoaded(const QUrl& url, const QImage& image)
{
    if (!image.isNull())
    {
        PhotoItem* photo = new PhotoItem(image, url.fileName(), this);
        photo->setPos(d->paste_scene_pos);

        d->paste_scene_pos += QPointF (20, 20);

        if (d->paste_scene_pos.x() >= this->sceneRect().bottomRight().x() ||
            d->paste_scene_pos.y() >= this->sceneRect().bottomRight().y() )
        {
            d->paste_scene_pos = this->sceneRect().topLeft();
        }

        this->addItem(photo);
    }
}

void PLEScene::calcSelectionBoundingRect()
{
    d->m_selected_items_path = QPainterPath();

    foreach (AbstractItemInterface* const item, d->m_selected_items.keys())
        d->m_selected_items_path = d->m_selected_items_path.united(item->mapToScene(item->shape()));
}

bool PLEScene::askAboutRemoving(int count)
{
    if (count)
    {
        int result = QMessageBox::question(qApp->activeWindow(),
                                           QObject::tr("Items deleting"),
                                           QObject::tr("Are you sure you want to delete selected item?", "Are you sure you want to delete %1 selected items?", count));
        if (result == QMessageBox::Yes)
            return true;
    }

    return false;
}


bool PLEScene::canDecode(const QMimeData* mimeData)
{
    if (PLEWindow::instance()->hasInterface() &&
        mimeData->hasFormat(QLatin1String("digikam/item-ids")))
        return true;

    QList<QUrl> urls = mimeData->urls();

    foreach (const QUrl& url, urls)
    {
        QImageReader ir(url.toLocalFile());

        if (!ir.canRead())
            return false;
    }

    return true;
}

bool PLEScene::isSelectionVisible()
{
    return d->m_selection_visible;
}

void PLEScene::setSelectionVisible(bool isVisible)
{
    d->m_selection_visible = isVisible;
}

} // namespace PhotoLayoutsEditor
