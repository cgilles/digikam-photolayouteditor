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

#include "photoitem.h"

// Qt includes

#include <QBuffer>
#include <QStyleOptionGraphicsItem>
#include <QFile>
#include <QImageReader>
#include <QGraphicsScene>
#include <QMimeData>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

// Local includes

#include "photoeffectsloader.h"
#include "imagedialog.h"
#include "bordersgroup.h"
#include "pleglobal.h"
#include "plewindow.h"
#include "imageloadingthread.h"
#include "progressevent.h"

#define EMPTY_FILL_COLOR QColor(255, 0, 0, 120)

using namespace Digikam;

namespace PhotoLayoutsEditor
{

class PhotoItemPixmapChangeCommand : public QUndoCommand
{
    QImage     m_image;
    PhotoItem* m_item;

public:

    PhotoItemPixmapChangeCommand(const QImage& image, PhotoItem* item, QUndoCommand* parent = nullptr)
        : QUndoCommand(QObject::tr("Image Change"), parent),
          m_image(image),
          m_item(item)
    {
    }

    PhotoItemPixmapChangeCommand(const QPixmap& pixmap, PhotoItem* item, QUndoCommand* parent = nullptr)
        : QUndoCommand(QObject::tr("Image Change"), parent),
          m_image(pixmap.toImage()),
          m_item(item)
    {
    }

    void redo() override
    {
        QImage temp = m_item->image();
        m_item->d->setImage(m_image);
        m_image = temp;
    }

    void undo() override
    {
        QImage temp = m_item->image();
        m_item->d->setImage(m_image);
        m_image = temp;
        m_item->update();
    }
};

class PhotoItemUrlChangeCommand : public QUndoCommand
{
    QUrl       m_url;
    PhotoItem* m_item;

public:

    PhotoItemUrlChangeCommand(const QUrl& url, PhotoItem* item, QUndoCommand* parent = nullptr)
        : QUndoCommand(QObject::tr("Image Path Change"), parent),
          m_url(url),
          m_item(item)
    {
    }

    void redo() override
    {
        run();
    }

    void undo() override
    {
        run();
    }

    void run()
    {
        QUrl temp = m_item->d->fileUrl();
        m_item->d->setFileUrl(m_url);
        m_url     = temp;
    }
};

class PhotoItemImagePathChangeCommand : public QUndoCommand
{
    PhotoItem*              m_item;
    QPainterPath            m_image_path;
//    CropShapeChangeCommand* command;

public:

    PhotoItemImagePathChangeCommand(PhotoItem* item, QUndoCommand* parent = nullptr)
        : QUndoCommand(QObject::tr("Image Shape Change"), parent),
          m_item(item),
          m_image_path(m_item->m_image_path)
//        , command(0)
    {
    }

    void redo() override
    {
        m_item->m_image_path = QPainterPath();
        m_item->m_image_path.addRect(m_item->m_temp_image.rect());
        m_item->recalcShape();
        m_item->update();
    }

    void undo() override
    {
        m_item->m_image_path = m_image_path;
        m_item->recalcShape();
        m_item->update();
    }
};

class PhotoItemImageMovedCommand : public QUndoCommand
{
    PhotoItem* m_item;
    QPointF    m_translation;
    bool       done;

    static PhotoItemImageMovedCommand* m_instance;

    PhotoItemImageMovedCommand(PhotoItem* item, QUndoCommand* parent = nullptr)
        : QUndoCommand(QObject::tr("Image Position Change"), parent),
          m_item(item),
          done(true)
    {
    }

public:

    static PhotoItemImageMovedCommand* instance(PhotoItem* item)
    {
        if (!m_instance)
            m_instance = new PhotoItemImageMovedCommand(item);

        return m_instance;
    }

    void translate(QPointF& translation)
    {
        m_translation += translation;
    }

    void redo() override
    {
        if (done)
            return;

        m_item->d->m_brush_transform.translate(m_translation.x(), m_translation.y());
        m_item->d->m_complete_path_transform.translate(m_translation.x(), m_translation.y());
        m_item->m_complete_path.translate(m_translation);
        m_item->update();
        done = !done;
    }

    void undo() override
    {
        if (!done)
            return;

        m_item->d->m_brush_transform.translate(-m_translation.x(), -m_translation.y());
        m_item->d->m_complete_path_transform.translate(-m_translation.x(), -m_translation.y());
        m_item->m_complete_path.translate(-m_translation);
        m_item->update();
        done = !done;
    }

    static void post()
    {
        if (!m_instance)
            return;

        PLE_PostUndoCommand(m_instance);
        m_instance = nullptr;
    }
};

PhotoItemImageMovedCommand* PhotoItemImageMovedCommand::m_instance = nullptr;

QString PhotoItem::PhotoItemPrivate::locateFile(const QString& filePath)
{
    QString resultPath = filePath;

    if (!resultPath.isEmpty())
    {
        // Try to open existing file

        if (!QFile::exists(resultPath))
        {
            int result = QMessageBox::question(qApp->activeWindow(),
                                               QObject::tr("File reading error"),
                                               QObject::tr("Can't find image file in this location:"
                                                    "\n%1"
                                                    "\n"
                                                    "\nWould you like to set new location of this file?"
                                                    "\nIf not this image will be removed from the composition.").arg(resultPath));
            if (result != QMessageBox::Yes)
            {
                resultPath.clear();
            }
            else
            {
                QUrl fileUrl(filePath);
                resultPath = ImageDialog::getImageURL(qApp->activeWindow(), fileUrl).toLocalFile();

                if (!QFile::exists(resultPath))
                    resultPath.clear();
            }
        }
    }

    return resultPath;
}

void PhotoItem::PhotoItemPrivate::setImage(const QImage& image)
{
    if (image.isNull() || image == m_image)
        return;

    m_image = image;
    m_item->refresh();
}

QImage& PhotoItem::PhotoItemPrivate::image()
{
    return m_image;
}

void PhotoItem::PhotoItemPrivate::setFileUrl(const QUrl& url)
{
    m_file_path = url;
}

QUrl& PhotoItem::PhotoItemPrivate::fileUrl()
{
    return m_file_path;
}

PhotoItem::PhotoItem(const QImage& photo, const QString& name, PLEScene* scene)
    : AbstractPhoto((name.isEmpty() ? QObject::tr("New image") : name), scene),
      m_highlight(false),
      d(new PhotoItemPrivate(this))
{
    setupItem(photo);
}

PhotoItem::PhotoItem(const QPainterPath& shape, const QString& name, PLEScene* scene)
    : AbstractPhoto((name.isEmpty() ? QObject::tr("New image") : name), scene),
      m_highlight(false),
      d(new PhotoItemPrivate(this)),
      m_image_path(shape)
{
    refresh();
}

PhotoItem::PhotoItem(const QString& name, PLEScene* scene)
    : AbstractPhoto((name.isEmpty() ? QObject::tr("New image") : name), scene),
      m_highlight(false),
      d(new PhotoItemPrivate(this))
{
    setupItem(QImage());
}

PhotoItem::~PhotoItem()
{
    delete d;
}

QDomDocument PhotoItem::toSvg() const
{
    QDomDocument document1  = AbstractPhoto::toSvg();
    QDomElement itemElement = document1.firstChildElement();
    itemElement.setAttribute(QLatin1String("class"), QLatin1String("PhotoItem"));

    // 'defs' tag
    QDomElement defs = document1.createElement(QLatin1String("defs"));
    defs.setAttribute(QLatin1String("class"), QLatin1String("data"));
    itemElement.appendChild(defs);

    // 'defs'-> ple:'data'
    QDomElement appNS = document1.createElementNS(PhotoLayoutsEditor::uri(), QLatin1String("data"));
    appNS.setPrefix(PhotoLayoutsEditor::name());
    defs.appendChild(appNS);

    if (!m_image_path.isEmpty())
    {
        // 'defs'-> ple:'data' ->'path'
        QDomDocument document2 = PhotoLayoutsEditor::pathToSvg(m_image_path);
        QDomElement path = document2.firstChildElement(QLatin1String("path"));
        path.setAttribute(QLatin1String("class"), QLatin1String("m_image_path"));
        path.setPrefix(PhotoLayoutsEditor::name());
        appNS.appendChild(document2.documentElement());
    }

    // 'defs'-> ple:'data' ->'transform'
    QDomElement transform = document1.createElement(QLatin1String("transform"));
    transform.setPrefix(PhotoLayoutsEditor::name());
    QString matrix = QLatin1String("matrix(") +
                     QString::number(this->transform().m11())+
                     QLatin1Char(',')+
                     QString::number(this->transform().m12())+
                     QLatin1Char(',')+
                     QString::number(this->transform().m21())+
                     QLatin1Char(',')+
                     QString::number(this->transform().m22())+
                     QLatin1Char(',')+
                     QString::number(this->transform().m31())+
                     QLatin1Char(',')+
                     QString::number(this->transform().m32())+
                     QLatin1Char(')');
    transform.setAttribute(QLatin1String("matrix"), matrix);
    appNS.appendChild(transform);

    if (!isEmpty())
    {
        QDomElement image = document1.createElementNS(PhotoLayoutsEditor::uri(), QLatin1String("image"));
        appNS.appendChild(image);

        bool embed = false;
        int result = QMessageBox::question(qApp->activeWindow(),
                                           QObject::tr("Saving: %1").arg(name()),
                                           QObject::tr("Do you want to embed images data?"
                                                "\n"
                                                "\nRemember that when you move or rename image files on your "
                                                "\ndisk or the storage device become unavailable, those "
                                                "\nimages become unavailable for %1 "
                                                "\nand this layout might become broken.").arg(QApplication::applicationName()));
        if (result != QMessageBox::Yes)
        {
            embed = true;
        }

        if ( (embed && !d->image().isNull()) || !d->fileUrl().isValid())
        {
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            d->image().save(&buffer, "PNG");
            image.appendChild( document1.createTextNode( QString::fromUtf8(byteArray.toBase64()) ) );
            image.setAttribute(QLatin1String("width"),QString::number(d->image().width()));
            image.setAttribute(QLatin1String("height"),QString::number(d->image().height()));
        }

        // Saving image path

        if (d->fileUrl().isValid())
        {
            image.setAttribute(QLatin1String("src"), d->fileUrl().url());
        }
    }
    else
    {
        itemElement.setAttribute(QLatin1String("visibility"), QLatin1String("hidden"));
    }

    return document1;
}

QDomDocument PhotoItem::toTemplateSvg() const
{
    QDomDocument document1 = AbstractPhoto::toTemplateSvg();
    QDomElement itemElement = document1.firstChildElement();
    itemElement.setAttribute(QLatin1String("class"), QLatin1String("PhotoItem"));

    // 'defs' tag

    QDomElement defs = document1.createElement(QLatin1String("defs"));
    defs.setAttribute(QLatin1String("class"), QLatin1String("data"));
    itemElement.appendChild(defs);

    // 'defs'-> ple:'data'

    QDomElement appNS = document1.createElementNS(PhotoLayoutsEditor::uri(), QLatin1String("data"));
    appNS.setPrefix(PhotoLayoutsEditor::name());
    defs.appendChild(appNS);

    if (!m_image_path.isEmpty())
    {
        // 'defs'-> ple:'data' ->'path'

        QDomDocument document2 = PhotoLayoutsEditor::pathToSvg(m_image_path);
        QDomElement path = document2.firstChildElement(QLatin1String("path"));
        path.setAttribute(QLatin1String("class"), QLatin1String("m_image_path"));
        path.setPrefix(PhotoLayoutsEditor::name());
        appNS.appendChild(document2.documentElement());
    }

    // 'defs'-> ple:'data' ->'transform'

    QDomElement transform = document1.createElement(QLatin1String("transform"));
    transform.setPrefix(PhotoLayoutsEditor::name());
    QString matrix = QLatin1String("matrix(")+
                     QString::number(this->transform().m11())+
                     QLatin1Char(',')+
                     QString::number(this->transform().m12())+
                     QLatin1Char(',')+
                     QString::number(this->transform().m21())+
                     QLatin1Char(',')+
                     QString::number(this->transform().m22())+
                     QLatin1Char(',')+
                     QString::number(this->transform().m31())+
                     QLatin1Char(',')+
                     QString::number(this->transform().m32())+
                     QLatin1Char(')');
    transform.setAttribute(QLatin1String("matrix"), matrix);
    appNS.appendChild(transform);

    return document1;
}

PhotoItem* PhotoItem::fromSvg(QDomElement& element)
{
    PhotoItem* item = new PhotoItem();

    if (item->AbstractPhoto::fromSvg(element))
    {
        // Gets data field

        QDomElement defs = element.firstChildElement(QLatin1String("defs"));

        while (!defs.isNull() && defs.attribute(QLatin1String("class")) != QLatin1String("data"))
            defs = defs.nextSiblingElement(QLatin1String("defs"));

        if (defs.isNull())
        {
            delete item;
            return nullptr;
        }

        QDomElement data = defs.firstChildElement(QLatin1String("data"));

        if (data.isNull())
        {
            delete item;
            return nullptr;
        }

        // m_image_path

        QDomElement path = data.firstChildElement(QLatin1String("path"));

        if (path.isNull())
        {
            delete item;
            return nullptr;
        }

        item->m_image_path = PhotoLayoutsEditor::pathFromSvg(path);

        if (item->m_image_path.isEmpty())
        {
            delete item;
            return nullptr;
        }

        // m_pixmap_original

        QDomElement image = data.firstChildElement(QLatin1String("image"));
        QString imageAttribute;
        QImage img;


        if      (!(imageAttribute = image.text()).isEmpty())
        {
            // Fullsize image is embedded in SVG file!

            img = QImage::fromData( QByteArray::fromBase64(imageAttribute.toLatin1()) );

            if (img.isNull())
            {
                delete item;
                return nullptr;
            }
        }
        else if ( !(imageAttribute = PhotoItemPrivate::locateFile( image.attribute(QLatin1String("xlink:href")) )).isEmpty() )
        {
            // Try to find file from path attribute

            QImageReader reader(imageAttribute);

            if (!reader.canRead())
            {
                delete item;
                return nullptr;
            }

            reader.setAutoDetectImageFormat(true);
            img = QImage(reader.size(), QImage::Format_ARGB32_Premultiplied);

            if (!reader.read(&img))
            {
                delete item;
                return nullptr;
            }
        }
        else
        {
            delete item;
            return nullptr;
        }

        item->d->setImage(img);

        return item;
    }

    delete item;
    return nullptr;
}

QDomDocument PhotoItem::svgVisibleArea() const
{
    QDomDocument document;

    if (!isEmpty())
    {
        // 'defs' -> 'g'

        QDomElement g = document.createElement(QLatin1String("g"));
        document.appendChild(g);
        QTransform transform = d->m_brush_transform;
        QString translate = QLatin1String("translate(")+
                            QString::number(pos().x())+
                            QLatin1Char(',')+
                            QString::number(pos().y())+
                            QLatin1Char(')');
        QString matrix = QLatin1String("matrix(")+
                         QString::number(transform.m11())+
                         QLatin1Char(',')+
                         QString::number(transform.m12())+
                         QLatin1Char(',')+
                         QString::number(transform.m21())+
                         QLatin1Char(',')+
                         QString::number(transform.m22())+
                         QLatin1Char(',')+
                         QString::number(transform.m31())+
                         QLatin1Char(',')+
                         QString::number(transform.m32())+
                         QLatin1Char(')');
        g.setAttribute(QLatin1String("transform"), translate + QLatin1Char(' ') + matrix);

        // 'defs' -> 'g' -> 'image'

        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        m_temp_image.save(&buffer, "PNG");
        QDomElement img = document.createElement(QLatin1String("image"));
        img.setAttribute(QLatin1String("width"),m_temp_image.width());
        img.setAttribute(QLatin1String("height"),m_temp_image.height());
        img.setAttribute(QLatin1String("xlink:href"), QLatin1String("data:image/png;base64,") + QString::fromUtf8(byteArray.toBase64()));
        g.appendChild(img);
    }

    return document;
}

QDomDocument PhotoItem::svgTemplateArea() const
{
    QDomDocument document1;

    if (!isEmpty())
    {
        // 'defs' -> 'g'

        QDomElement g = document1.createElement(QLatin1String("g"));
        document1.appendChild(g);
        QTransform transform = d->m_brush_transform;
        QString translate = QLatin1String("translate(")+
                            QString::number(pos().x())+
                            QLatin1Char(',')+
                            QString::number(pos().y())+
                            QLatin1Char(')');
        QString matrix = QLatin1String("matrix(")+
                         QString::number(transform.m11())+
                         QLatin1Char(',')+
                         QString::number(transform.m12())+
                         QLatin1Char(',')+
                         QString::number(transform.m21())+
                         QLatin1Char(',')+
                         QString::number(transform.m22())+
                         QLatin1Char(',')+
                         QString::number(transform.m31())+
                         QLatin1Char(',')+
                         QString::number(transform.m32())+
                         QLatin1Char(')');
        g.setAttribute(QLatin1String("transform"), translate + QLatin1Char(' ') + matrix);

        // 'defs' -> 'g' -> 'path'

        QDomDocument document2 = PhotoLayoutsEditor::pathToSvg(m_image_path);
        QDomElement path      = document2.firstChildElement(QLatin1String("path"));
        path.setAttribute(QLatin1String("opacity"), 100);
        path.setAttribute(QLatin1String("fill"), QLatin1String("#ff0000"));
        g.appendChild(path);
    }

    return document1;
}

void PhotoItem::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    if (PLEWindow::instance()->hasInterface() &&
        mimeData->hasFormat(QLatin1String("digikam/item-ids")))
    {
        QList<QUrl> urls;
        QByteArray ba = mimeData->data(QLatin1String("digikam/item-ids"));
        QDataStream ds(&ba, QIODevice::ReadOnly);
        ds >> urls;
        event->setAccepted( (urls.count() == 1) );

        if (urls.count() == 1)
            event->setDropAction( Qt::CopyAction );
        else
            event->setDropAction( Qt::IgnoreAction );
    }
    else if (mimeData->hasFormat(QLatin1String("text/uri-list")))
    {
        QList<QUrl> urls = mimeData->urls();
        event->setAccepted( (urls.count() == 1) );

        if (urls.count() == 1)
            event->setDropAction( Qt::CopyAction );
        else
            event->setDropAction( Qt::IgnoreAction );
    }

    setHighlightItem( event->isAccepted() );
}

void PhotoItem::dragLeaveEvent(QGraphicsSceneDragDropEvent* /*event*/)
{
    setHighlightItem(false);
}

void PhotoItem::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    if (PLEWindow::instance()->hasInterface() &&
        mimeData->hasFormat(QLatin1String("digikam/item-ids")))
    {
        QList<QUrl> urls;
        QByteArray ba = mimeData->data(QLatin1String("digikam/item-ids"));
        QDataStream ds(&ba, QIODevice::ReadOnly);
        ds >> urls;
        event->setAccepted( (urls.count() == 1) );

        if (urls.count() == 1)
            event->setDropAction( Qt::CopyAction );
        else
            event->setDropAction( Qt::IgnoreAction );
    }
    else if (mimeData->hasFormat(QLatin1String("text/uri-list")))
    {
        QList<QUrl> urls = mimeData->urls();
        event->setAccepted( (urls.count() == 1) );

        if (urls.count() == 1)
            event->setDropAction( Qt::CopyAction );
        else
            event->setDropAction( Qt::IgnoreAction );
    }

    setHighlightItem( event->isAccepted() );
}

void PhotoItem::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    QImage img;
    const QMimeData* mimeData = event->mimeData();

    if ( PLEWindow::instance()->hasInterface() &&
         mimeData->hasFormat(QLatin1String("digikam/item-ids")))
    {
        QList<QUrl> urls;
        QByteArray ba = mimeData->data(QLatin1String("digikam/item-ids"));
        QDataStream ds(&ba, QIODevice::ReadOnly);
        ds >> urls;

        if (urls.count() == 1)
            setImageUrl(urls.at(0));
    }
    else if (mimeData->hasFormat(QLatin1String("text/uri-list")))
    {
        QList<QUrl> urls = mimeData->urls();

        if (urls.count() == 1)
            setImageUrl(urls.at(0));
    }

    setHighlightItem(false);
    event->setAccepted( !img.isNull() );
}

void PhotoItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if ((event->modifiers() & Qt::ControlModifier) && (event->buttons() & Qt::LeftButton))
        d->m_image_moving = true;
    else
        AbstractPhoto::mousePressEvent(event);
}

void PhotoItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    event->setAccepted(false);

    if (d->m_image_moving)
    {
        if ((event->modifiers() & Qt::ControlModifier) && (event->buttons() & Qt::LeftButton))
        {
            QPointF p = event->pos() - event->lastPos();
            d->m_brush_transform.translate(p.x(), p.y());
            d->m_complete_path_transform.translate(p.x(), p.y());
            m_complete_path.translate(p);
            PhotoItemImageMovedCommand::instance(this)->translate(p);
            update();
        }
        else
        {
            PhotoItemImageMovedCommand::post();
        }

        event->setAccepted(true);
    }
    else
    {
        AbstractPhoto::mouseMoveEvent(event);
    }
}

void PhotoItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (d->m_image_moving)
    {
        PhotoItemImageMovedCommand::post();
        d->m_image_moving = false;
    }
    else
    {
        AbstractPhoto::mouseReleaseEvent(event);
    }
}

QImage& PhotoItem::image()
{
    return d->m_image;
}

const QImage& PhotoItem::image() const
{
    return d->m_image;
}

void PhotoItem::setImage(const QImage& image)
{
    qDebug() << "setImage();";

    if (image.isNull())
        return;

    PLEWindow::instance()->beginUndoCommandGroup(QObject::tr("Image Change"));
    PLE_PostUndoCommand(new PhotoItemPixmapChangeCommand(image, this));

    if (cropShape().isEmpty())
        setCropShape( m_image_path );

    PLE_PostUndoCommand(new PhotoItemImagePathChangeCommand(this));
    PLEWindow::instance()->endUndoCommandGroup();
}

void PhotoItem::imageLoaded(const QUrl& url, const QImage& image)
{
    if (image.isNull())
        return;

    PLEWindow::instance()->beginUndoCommandGroup(QObject::tr("Image Change"));
    PLE_PostUndoCommand(new PhotoItemPixmapChangeCommand(image, this));

    if (cropShape().isEmpty())
        setCropShape( m_image_path );

    PLE_PostUndoCommand(new PhotoItemImagePathChangeCommand(this));
    PLE_PostUndoCommand(new PhotoItemUrlChangeCommand(url, this));
    PLEWindow::instance()->endUndoCommandGroup();
}

void PhotoItem::setImageUrl(const QUrl& url)
{
    ImageLoadingThread* ilt = new ImageLoadingThread(this);
    ilt->setImagesUrls(QList<QUrl>() << url);
    connect(ilt, SIGNAL(imageLoaded(QUrl,QImage)), this, SLOT(imageLoaded(QUrl,QImage)));
    ilt->start();
}

void PhotoItem::updateIcon()
{
    QPixmap temp(m_temp_image.size());

    if (m_temp_image.isNull())
        temp = QPixmap(48,48);

    temp.fill(Qt::transparent);

    QPainter p(&temp);

    if (!m_temp_image.isNull())
    {
        p.fillPath(itemOpaqueArea(), QBrush(m_temp_image));
        p.end();
        temp = temp.scaled(48,48,Qt::KeepAspectRatio);
        p.begin(&temp);
    }

    QPen pen(Qt::gray,1);
    pen.setCosmetic(true);
    p.setPen(pen);
    p.drawRect( QRect(QPoint(0,0), temp.size()-QSize(1,1)) );
    p.end();
    setIcon(QIcon(temp));
}

void PhotoItem::fitToRect(const QRect& rect)
{
    // Scaling if to big
    QSize s = d->image().size();
    QRect r = d->image().rect();

    if (rect.isValid() && (rect.width()<s.width() || rect.height()<s.height()))
    {
        s.scale(rect.size()*0.8, Qt::KeepAspectRatio);
        r.setSize(s);
    }

    QPainterPath p;
    p.addRect(r);
    m_image_path = p;
    m_image_path = m_image_path.simplified();
    recalcShape();

    // Create effective pixmap

    refresh();
}

void PhotoItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->fillPath(itemOpaqueArea(), EMPTY_FILL_COLOR);

    if (!m_temp_image.isNull())
    {
        QBrush b(m_temp_image);
        b.setTransform(d->m_brush_transform);
        painter->fillPath(itemOpaqueArea() & m_complete_path, b);
    }

    AbstractPhoto::paint(painter, option, widget);

    // Highlight item

    if (m_highlight)
    {
        painter->fillPath(shape(), QColor(255,0,0,100));
    }
}

void PhotoItem::refreshItem()
{
    if (d->image().isNull())
        return;

    m_temp_image = effectsGroup()->apply( d->image().scaled(m_image_path.boundingRect().size().toSize(),
                                                            Qt::KeepAspectRatioByExpanding,
                                                            Qt::SmoothTransformation));

    updateIcon();
    recalcShape();
    update();
}

QtAbstractPropertyBrowser* PhotoItem::propertyBrowser()
{
    return nullptr; /// TODO
}

bool PhotoItem::isEmpty() const
{
    return d->m_image.isNull();
}

void PhotoItem::setupItem(const QImage& image)
{
    if (image.isNull())
        return;

    d->setImage(image);

    // Scaling if to big

    if (scene())
        fitToRect(scene()->sceneRect().toRect());
    else
        fitToRect(image.rect());

    // Create effective pixmap

    refresh();

    setFlag(QGraphicsItem::ItemIsSelectable);
}

void PhotoItem::recalcShape()
{
    m_complete_path = m_image_path;
    d->m_brush_transform = QTransform();
}

bool PhotoItem::highlightItem()
{
    return m_highlight;
}

void PhotoItem::setHighlightItem(bool isHighlighted)
{
    if (m_highlight == isHighlighted)
        return;

    m_highlight = isHighlighted;
    update();
}

} // namespace PhotoLayoutsEditor
