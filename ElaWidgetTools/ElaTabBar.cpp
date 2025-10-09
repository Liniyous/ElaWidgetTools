#include "ElaTabBar.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

#include "ElaTabBarPrivate.h"
#include "ElaTabBarStyle.h"
#include "private/qtabbar_p.h"
#include <QTimer>
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
    d->_style = new ElaTabBarStyle(style());
    setStyle(d->_style);
    d->_tabBarPrivate = dynamic_cast<QTabBarPrivate*>(this->QTabBar::d_ptr.data());
}

ElaTabBar::~ElaTabBar()
{
    Q_D(ElaTabBar);
    delete d->_style;
}

void ElaTabBar::setTabSize(QSize tabSize)
{
    Q_D(ElaTabBar);
    d->_style->setTabSize(tabSize);
}

QSize ElaTabBar::getTabSize() const
{
    Q_D(const ElaTabBar);
    return d->_style->getTabSize();
}

QSize ElaTabBar::sizeHint() const
{
    QSize oldSize = QTabBar::sizeHint();
    QSize newSize = oldSize;
    newSize.setWidth(parentWidget()->width());
    return oldSize.expandedTo(newSize);
}

void ElaTabBar::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaTabBar);
    QPoint currentPos = event->pos();
    if (objectName() == "ElaCustomTabBar" && count() == 1)
    {
        if (!d->_mimeData)
        {
            d->_mimeData = new QMimeData();
            d->_mimeData->setProperty("DragType", "ElaTabBarDrag");
            d->_mimeData->setProperty("ElaTabBarObject", QVariant::fromValue(this));
            d->_mimeData->setProperty("TabSize", d->_style->getTabSize());
            d->_mimeData->setProperty("IsFloatWidget", true);
            QRect currentTabRect = tabRect(currentIndex());
            d->_mimeData->setProperty("DragPos", QPoint(currentPos.x() - currentTabRect.x(), currentPos.y() - currentTabRect.y()));
            Q_EMIT tabDragCreate(d->_mimeData);
            d->_mimeData = nullptr;
        }
    }
    else
    {
        QRect moveRect = rect();
        moveRect.adjust(0, -height(), 0, height());
        if (currentPos.x() < 0 || currentPos.x() > width() || currentPos.y() > moveRect.bottom() || currentPos.y() < moveRect.y())
        {
            if (!d->_mimeData)
            {
                d->_mimeData = new QMimeData();
                d->_mimeData->setProperty("DragType", "ElaTabBarDrag");
                d->_mimeData->setProperty("ElaTabBarObject", QVariant::fromValue(this));
                d->_mimeData->setProperty("TabSize", d->_style->getTabSize());
                Q_EMIT tabDragCreate(d->_mimeData);
                d->_mimeData = nullptr;
            }
        }
    }
    QTabBar::mouseMoveEvent(event);
}

void ElaTabBar::dragEnterEvent(QDragEnterEvent* event)
{
    Q_D(ElaTabBar);
    if (event->mimeData()->property("DragType").toString() == "ElaTabBarDrag")
    {
        event->acceptProposedAction();
        auto mimeData = const_cast<QMimeData*>(event->mimeData());
        d->_mimeData = mimeData;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        mimeData->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
        mimeData->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
        Q_EMIT tabDragEnter(mimeData);
        QTimer::singleShot(10, this, [=]() {
            QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(tabRect(currentIndex()).x() + 110, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            QApplication::sendEvent(this, &pressEvent);
            QMouseEvent moveEvent(QEvent::MouseMove, QPoint(event->pos().x(), 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            QApplication::sendEvent(this, &moveEvent);
        });
    }
    QTabBar::dragEnterEvent(event);
}

void ElaTabBar::dragMoveEvent(QDragMoveEvent* event)
{
    Q_D(ElaTabBar);
    if (event->mimeData()->property("DragType").toString() == "ElaTabBarDrag")
    {
        QMouseEvent moveEvent(QEvent::MouseMove, QPoint(event->pos().x(), 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QApplication::sendEvent(this, &moveEvent);
    }
    QWidget::dragMoveEvent(event);
}

void ElaTabBar::dragLeaveEvent(QDragLeaveEvent* event)
{
    Q_D(ElaTabBar);
    if (d->_mimeData)
    {
        Q_EMIT tabDragLeave(d->_mimeData);
        d->_mimeData = nullptr;
    }
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this, &releaseEvent);
    QTabBar::dragLeaveEvent(event);
}

void ElaTabBar::dropEvent(QDropEvent* event)
{
    Q_D(ElaTabBar);
    d->_mimeData = nullptr;
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this, &releaseEvent);
    if (objectName() != "ElaCustomTabBar")
    {
        QMimeData* data = const_cast<QMimeData*>(event->mimeData());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        data->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
        data->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
        Q_EMIT tabDragDrop(data);
    }
    event->accept();
    QTabBar::dropEvent(event);
}

void ElaTabBar::wheelEvent(QWheelEvent* event)
{
    QTabBar::wheelEvent(event);
    event->accept();
}

void ElaTabBar::paintEvent(QPaintEvent* event)
{
    Q_D(ElaTabBar);
    QSize tabSize = d->_style->getTabSize();
    for (int i = 0; i < d->_tabBarPrivate->tabList.size(); i++)
    {
#if (QT_VERSION > QT_VERSION_CHECK(6, 0, 0))
        d->_tabBarPrivate->tabList[i]->rect = QRect(tabSize.width() * i, d->_tabBarPrivate->tabList[i]->rect.y(), tabSize.width(), tabSize.height());
#else
        d->_tabBarPrivate->tabList[i].rect = QRect(tabSize.width() * i, d->_tabBarPrivate->tabList[i].rect.y(), tabSize.width(), tabSize.height());
#endif
    }
    d->_tabBarPrivate->layoutWidgets();
    QTabBar::paintEvent(event);
}