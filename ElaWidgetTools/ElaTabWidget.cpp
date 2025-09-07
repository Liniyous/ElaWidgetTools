#include "ElaTabWidget.h"

#include "ElaTabBar.h"
#include "ElaTabWidgetPrivate.h"
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
ElaTabWidget::ElaTabWidget(QWidget* parent)
    : QTabWidget(parent), d_ptr(new ElaTabWidgetPrivate())
{
    Q_D(ElaTabWidget);
    d->q_ptr = this;
    _pIsTabTransparent = false;
    setObjectName("ElaTabWidget");
    setAcceptDrops(true);
    ElaTabBar* tabBar = new ElaTabBar(this);
    setTabBar(tabBar);
    connect(tabBar, &ElaTabBar::tabDragCreate, d, &ElaTabWidgetPrivate::onTabDragCreate);
    connect(tabBar, &ElaTabBar::tabDragDrop, d, &ElaTabWidgetPrivate::onTabDragDrop);
    connect(tabBar, &ElaTabBar::tabCloseRequested, d, &ElaTabWidgetPrivate::onTabCloseRequested);
}

ElaTabWidget::~ElaTabWidget()
{
    Q_D(ElaTabWidget);
    for (auto widget: d->_allTabWidgetList)
    {
        auto originTabWidgetVariant = widget->property("ElaOriginTabWidget");
        if (originTabWidgetVariant.isValid() && originTabWidgetVariant.value<ElaTabWidget*>() == this)
        {
            widget->setProperty("ElaOriginTabWidget", QVariant());
        }
    }
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
    if (!_pIsTabTransparent)
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
    if (event->mimeData()->property("ElaTabWidgetObject").value<ElaTabWidget*>() != this)
    {
        QMimeData* data = const_cast<QMimeData*>(event->mimeData());
        data->setProperty("TabDropIndex", count());
        d->onTabDragDrop(event->mimeData());
        event->accept();
    }
    QTabWidget::dropEvent(event);
}

void ElaTabWidget::tabInserted(int index)
{
    Q_D(ElaTabWidget);
    QWidget* tabWidget = widget(index);
    if (!tabWidget->property("ElaOriginTabWidget").isValid())
    {
        d->_allTabWidgetList.append(widget(index));
    }
    QTabWidget::tabInserted(index);
}
