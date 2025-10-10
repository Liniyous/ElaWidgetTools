#include "ElaTabWidget.h"

#include "ElaTabBar.h"
#include "ElaTabWidgetPrivate.h"
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
Q_PROPERTY_CREATE_Q_CPP(ElaTabWidget, bool, IsTabTransparent);
Q_PROPERTY_CREATE_Q_CPP(ElaTabWidget, bool, IsContainerAcceptDrops);
ElaTabWidget::ElaTabWidget(QWidget* parent)
    : QTabWidget(parent), d_ptr(new ElaTabWidgetPrivate())
{
    Q_D(ElaTabWidget);
    d->q_ptr = this;
    d->_pIsContainerAcceptDrops = false;
    d->_pIsTabTransparent = false;
    setObjectName("ElaTabWidget");
    setAcceptDrops(true);
    d->_tabBar = new ElaTabBar(this);
    setTabBar(d->_tabBar);
    connect(d->_tabBar, &ElaTabBar::tabDragCreate, d, &ElaTabWidgetPrivate::onTabDragCreate);
    connect(d->_tabBar, &ElaTabBar::tabDragEnter, d, &ElaTabWidgetPrivate::onTabDragEnter);
    connect(d->_tabBar, &ElaTabBar::tabDragLeave, d, &ElaTabWidgetPrivate::onTabDragLeave);
    connect(d->_tabBar, &ElaTabBar::tabDragDrop, d, &ElaTabWidgetPrivate::onTabDragDrop);
    connect(d->_tabBar, &ElaTabBar::tabCloseRequested, d, &ElaTabWidgetPrivate::onTabCloseRequested);
}

ElaTabWidget::~ElaTabWidget()
{
    Q_D(ElaTabWidget);
    d->_clearAllTabWidgetList();
}

void ElaTabWidget::setTabSize(QSize tabSize)
{
    Q_D(ElaTabWidget);
    d->_tabBar->setTabSize(tabSize);
}

QSize ElaTabWidget::getTabSize() const
{
    Q_D(const ElaTabWidget);
    return d->_tabBar->getTabSize();
}

void ElaTabWidget::setTabPosition(TabPosition position)
{
    if (position == QTabWidget::North || position == QTabWidget::South)
    {
        QTabWidget::setTabPosition(position);
    }
}

void ElaTabWidget::paintEvent(QPaintEvent* event)
{
    Q_D(ElaTabWidget);
    if (!d->_pIsTabTransparent)
    {
        QTabWidget::paintEvent(event);
    }
}

void ElaTabWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->property("DragType").toString() == "ElaTabBarDrag")
    {
        event->acceptProposedAction();
    }
    QTabWidget::dragEnterEvent(event);
}

void ElaTabWidget::dropEvent(QDropEvent* event)
{
    Q_D(ElaTabWidget);
    if (d->_pIsContainerAcceptDrops && event->mimeData()->property("ElaTabWidgetObject").value<ElaTabWidget*>() != this)
    {
        QMimeData* data = const_cast<QMimeData*>(event->mimeData());
        data->setProperty("TabDropIndex", count());
        d->onTabDragDrop(data);
        event->accept();
    }
    QTabWidget::dropEvent(event);
}

void ElaTabWidget::tabInserted(int index)
{
    Q_D(ElaTabWidget);
    QWidget* tabWidget = widget(index);
    if (!tabWidget->property("IsMetaWidget").toBool() && !tabWidget->property("ElaOriginTabWidget").isValid())
    {
        d->_allTabWidgetList.append(widget(index));
    }
    QTabWidget::tabInserted(index);
}
