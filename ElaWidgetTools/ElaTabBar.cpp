#include "ElaTabBar.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

#include "ElaTabBarPrivate.h"
#include "ElaTabBarStyle.h"
ElaTabBar::ElaTabBar(QWidget* parent)
    : QTabBar(parent), d_ptr(new ElaTabBarPrivate())
{
    Q_D(ElaTabBar);
    d->q_ptr = this;
    setObjectName("ElaTabBar");
    setStyleSheet("#ElaTabBar{background-color:transparent;}");
    setTabsClosable(true);
    setMovable(true);
    setAcceptDrops(true);
    setStyle(new ElaTabBarStyle(style()));
}

ElaTabBar::~ElaTabBar()
{
    delete this->style();
}

void ElaTabBar::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaTabBar);
    QTabBar::mousePressEvent(event);
    d->_lastDragPix = grab(tabRect(currentIndex()));
    Q_EMIT tabBarPress(currentIndex());
}

void ElaTabBar::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaTabBar);
    QRect moveRect = rect();
    moveRect.adjust(0, -height(), 0, height());
    QPoint currentPos = event->pos();
    if (currentPos.y() > moveRect.bottom() || currentPos.y() < moveRect.y())
    {
        QDrag* drag = new QDrag(this);
        drag->setPixmap(d->_lastDragPix);
        qreal pixRatio = d->_lastDragPix.devicePixelRatioF();
        QPoint pixCenter = d->_lastDragPix.rect().center();
        drag->setHotSpot(pixCenter / pixRatio);
        QMimeData* data = new QMimeData;
        data->setProperty("DragType", "ElaTabBarDrag");
        data->setProperty("TabIndex", currentIndex());
        data->setProperty("TabIcon", tabIcon(currentIndex()));
        data->setProperty("TabText", tabText(currentIndex()));
        data->setProperty("ElaTabBarObject", QVariant::fromValue(this));
        data->setProperty("QDragObject", QVariant::fromValue(drag));
        drag->setMimeData(data);
        Q_EMIT tabDragCreate(drag);
        drag->deleteLater();
    }
    QTabBar::mouseMoveEvent(event);
}

void ElaTabBar::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->property("DragType").toString() == "ElaTabBarDrag")
    {
        event->acceptProposedAction();
    }
    QTabBar::dragEnterEvent(event);
}

void ElaTabBar::dropEvent(QDropEvent* event)
{
    if (event->mimeData()->property("ElaTabBarObject").value<ElaTabBar*>() != this)
    {
        event->accept();
        QMimeData* data = const_cast<QMimeData*>(event->mimeData());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        data->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
        data->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
        Q_EMIT tabDragDrop(data);
    }
    QTabBar::dropEvent(event);
}

void ElaTabBar::wheelEvent(QWheelEvent* event)
{
    QTabBar::wheelEvent(event);
    event->accept();
}
