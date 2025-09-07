#include "ElaTabWidgetPrivate.h"

#include "ElaCustomTabWidget.h"
#include "ElaTabBar.h"
#include "ElaTabWidget.h"
#include <QApplication>
#include <QDebug>
#include <QMimeData>
#include <QMouseEvent>
ElaTabWidgetPrivate::ElaTabWidgetPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaTabWidgetPrivate::~ElaTabWidgetPrivate()
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
    QVariant originTabWidgetVariant = dragWidget->property("ElaOriginTabWidget");
    ElaTabBar* tabBarObject = mimeData->property("ElaTabBarObject").value<ElaTabBar*>();
    if (!originTabWidgetVariant.isValid() && q->objectName() != "ElaCustomTabWidget")
    {
        dragWidget->setProperty("ElaOriginTabWidget", QVariant::fromValue<ElaTabWidget*>(q));
    }
    mimeData->setProperty("DragWidget", QVariant::fromValue(dragWidget));
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(tabBarObject, &releaseEvent);
    if (drag->exec() == Qt::IgnoreAction)
    {
        // 创建新窗口
        ElaTabBar* originCustomTabBar = tabBarObject;
        if (originCustomTabBar && originCustomTabBar->objectName() == "ElaCustomTabBar")
        {
            originCustomTabBar->removeTab(index);
        }
        QWidget* floatParentWidget = originTabWidgetVariant.value<ElaTabWidget*>();
        if (!floatParentWidget)
        {
            floatParentWidget = dragWidget->property("ElaFloatParentWidget").value<QWidget*>();
        }
        ElaCustomTabWidget* floatWidget = new ElaCustomTabWidget(floatParentWidget);
        ElaTabBar* customTabBar = floatWidget->getCustomTabBar();
        dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<ElaTabBar*>(customTabBar));
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
    else if (!originTabWidget && q->objectName() == "ElaCustomTabWidget")
    {
        _customTabBar->removeTab(index);
        q->removeTab(index);
        closeWidget->deleteLater();
    }
    else
    {
        q->removeTab(index);
        closeWidget->deleteLater();
    }
}
