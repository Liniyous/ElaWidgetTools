#include "ElaTabWidgetPrivate.h"

#include <QApplication>
#include <QMimeData>
#include <QMouseEvent>

#include "ElaCustomTabWidget.h"
#include "ElaTabBar.h"
#include "ElaTabWidget.h"
ElaTabWidgetPrivate::ElaTabWidgetPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaTabWidgetPrivate::~ElaTabWidgetPrivate()
{
}

void ElaTabWidgetPrivate::onTabBarPress(int index)
{
}

void ElaTabWidgetPrivate::onTabDragCreate(QDrag* drag)
{
    Q_Q(ElaTabWidget);
    QMimeData* mimeData = drag->mimeData();
    mimeData->setProperty("ElaTabWidgetObject", QVariant::fromValue(q));
    int index = mimeData->property("TabIndex").toInt();
    QString tabText = q->tabText(index);
    QIcon tabIcon = q->tabIcon(index);
    QWidget* dragWidget = q->widget(index);
    QVariant originTabWidget = dragWidget->property("ElaOriginTabWidget");
    if (!originTabWidget.isValid())
    {
        dragWidget->setProperty("ElaOriginTabWidget", QVariant::fromValue<ElaTabWidget*>(q));
    }
    mimeData->setProperty("DragWidget", QVariant::fromValue(dragWidget));
    drag->setMimeData(mimeData);
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(mimeData->property("ElaTabBarObject").value<ElaTabBar*>(), &releaseEvent);
    if (drag->exec() == Qt::IgnoreAction)
    {
        // 创建新窗口
        ElaTabBar* originCustomTabBar = mimeData->property("ElaTabBarObject").value<ElaTabBar*>();
        if (originCustomTabBar && originCustomTabBar->objectName() == "ElaCustomTabBar")
        {
            originCustomTabBar->removeTab(index);
        }
        ElaTabWidget* originTabWidget = dragWidget->property("ElaOriginTabWidget").value<ElaTabWidget*>();
        ElaCustomTabWidget* floatWidget = new ElaCustomTabWidget(originTabWidget);
        ElaTabBar* customTabBar = floatWidget->getCustomTabBar();
        dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<ElaTabBar*>(customTabBar));
        ElaTabWidget* customTabWidget = floatWidget->getCustomTabWidget();
        customTabWidget->d_ptr->_customTabBar = customTabBar;
        connect(customTabBar, &ElaTabBar::tabBarPress, customTabWidget->d_func(), &ElaTabWidgetPrivate::onTabBarPress);
        connect(customTabBar, &ElaTabBar::tabDragCreate, customTabWidget->d_func(), &ElaTabWidgetPrivate::onTabDragCreate);
        connect(customTabBar, &ElaTabBar::tabDragDrop, customTabWidget->d_func(), &ElaTabWidgetPrivate::onTabDragDrop);
        floatWidget->addTab(dragWidget, tabIcon, tabText);
        floatWidget->show();
        QPoint cursorPoint = QCursor::pos();
        floatWidget->move(cursorPoint.x() - floatWidget->width() / 2, cursorPoint.y() - 40);
    }
}

void ElaTabWidgetPrivate::onTabDragDrop(const QMimeData* mimeData)
{
    Q_Q(ElaTabWidget);
    QWidget* dragWidget = mimeData->property("DragWidget").value<QWidget*>();
    QString tabText = mimeData->property("TabText").toString();
    QIcon tabIcon = mimeData->property("TabIcon").value<QIcon>();
    ElaTabBar* customTabBar = mimeData->property("ElaTabBarObject").value<ElaTabBar*>();
    int index = mimeData->property("TabIndex").toInt();
    int dropIndex = mimeData->property("TabDropIndex").toInt();
    if (customTabBar && customTabBar->objectName() == "ElaCustomTabBar")
    {
        customTabBar->removeTab(index);
    }
    q->insertTab(dropIndex, dragWidget, tabIcon, tabText);
    if (_customTabBar)
    {
        dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<ElaTabBar*>(_customTabBar));
        _customTabBar->insertTab(dropIndex, tabIcon, tabText);
    }
}

void ElaTabWidgetPrivate::onTabCloseRequested(int index)
{
    Q_Q(ElaTabWidget);
    QWidget* closeWidget = q->widget(index);
    ElaTabWidget* originTabWidget = closeWidget->property("ElaOriginTabWidget").value<ElaTabWidget*>();
    if (originTabWidget && originTabWidget != q)
    {
        ElaTabBar* customTabBar = closeWidget->property("CurrentCustomBar").value<ElaTabBar*>();
        if (customTabBar)
        {
            customTabBar->removeTab(index);
            closeWidget->setProperty("CurrentCustomBar", QVariant::fromValue<ElaTabBar*>(nullptr));
        }
        originTabWidget->addTab(closeWidget, q->tabIcon(index), q->tabText(index));
    }
    else
    {
        q->removeTab(index);
        // fuqaq : deleteLater due to project cant store widget status then discard it
        // closeWidget->deleteLater();
    }
}
