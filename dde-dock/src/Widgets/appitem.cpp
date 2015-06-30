#include "appitem.h"

AppItem::AppItem(QWidget *parent) :
    AbstractDockItem(parent)
{
    setAcceptDrops(true);
    resize(itemWidth, itemHeight);
    initBackground();
}

AppItem::AppItem(QString title, QWidget *parent):
    AbstractDockItem(parent)
{
    m_itemTitle = title;

    setAcceptDrops(true);
    resize(itemWidth, itemHeight);
    initBackground();
}

AppItem::AppItem(QString title, QString iconPath, QWidget *parent) :
    AbstractDockItem(parent)
{
    m_itemTitle = title;
    m_itemIconPath = iconPath;

    setAcceptDrops(true);
    resize(itemWidth, itemHeight);
    initBackground();
    setIcon(m_itemIconPath);
}

QPoint AppItem::getNextPos()
{
    return this->nextPos;
}

void AppItem::setNextPos(const QPoint &value)
{
    this->nextPos = value;
}

void AppItem::setNextPos(int x, int y)
{
    this->nextPos.setX(x);
    this->nextPos.setY(y);
}

void AppItem::resizeResources()
{
    if (m_appIcon != NULL)
    {
        m_appIcon->resize(DockConstants::getInstants()->getIconSize(),
                          DockConstants::getInstants()->getIconSize());
        m_appIcon->move(width() / 2 - m_appIcon->width() / 2,
                        height() / 2 - m_appIcon->height() / 2);
    }

    if (appBackground != NULL)
    {
        appBackground->resize(width(), height());
        appBackground->move(0,0);
    }
}

void AppItem::initBackground()
{
    appBackground = new AppBackground(this);
    appBackground->resize(width(), height());
    appBackground->move(0,0);
}

void AppItem::mousePressEvent(QMouseEvent * event)
{
    //qWarning() << "mouse press...";
    emit mousePress(event->globalX(), event->globalY(),this);
}

void AppItem::mouseReleaseEvent(QMouseEvent * event)
{
//    qWarning() << "mouse release...";
    emit mouseRelease(event->globalX(), event->globalY(),this);
}

void AppItem::mouseDoubleClickEvent(QMouseEvent * event)
{
    emit mouseDoubleClick(this);
}

void AppItem::mouseMoveEvent(QMouseEvent *event)
{
    //this event will only execp onec then handle by Drag
    emit dragStart(this);

    Qt::MouseButtons btn = event->buttons();
    if(btn == Qt::LeftButton)
    {
        QDrag* drag = new QDrag(this);
        QMimeData* data = new QMimeData();
        QImage dataImg(m_itemIconPath);
        data->setImageData(QVariant(dataImg));
        drag->setMimeData(data);

        QPixmap pixmap(m_itemIconPath);
        drag->setPixmap(pixmap);

        drag->setHotSpot(QPoint(15,15));

        drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::MoveAction);
    }
}

void AppItem::enterEvent(QEvent *event)
{
    emit mouseEntered(this);
}

void AppItem::leaveEvent(QEvent *event)
{
    emit mouseExited(this);
}

void AppItem::dragEnterEvent(QDragEnterEvent *event)
{
    emit dragEntered(event,this);

    AppItem *tmpItem = NULL;
    tmpItem = dynamic_cast<AppItem *>(event->source());
    if (tmpItem)
    {
//        qWarning()<< "[Info:]" << "Brother Item.";
    }
    else
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void AppItem::dragLeaveEvent(QDragLeaveEvent *event)
{
    emit dragExited(event,this);
}

void AppItem::dropEvent(QDropEvent *event)
{
    qWarning() << "Item get drop:" << event->pos();
    emit drop(event,this);
}

AppItem::~AppItem()
{

}

