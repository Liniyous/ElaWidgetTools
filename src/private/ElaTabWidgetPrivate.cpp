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
    mimeData->setProperty("DragWidget", QVariant::fromValue(q->widget(index)));
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
        ElaCustomTabWidget* floatWidget = new ElaCustomTabWidget();
        ElaTabBar* customTabBar = floatWidget->getCustomTabBar();
        ElaTabWidget* customTabWidget = floatWidget->getCustomTabWidget();
        customTabWidget->d_ptr->_customTabBar = customTabBar;
        connect(customTabBar, &ElaTabBar::tabBarPress, customTabWidget->d_func(), &ElaTabWidgetPrivate::onTabBarPress);
        connect(customTabBar, &ElaTabBar::tabDragCreate, customTabWidget->d_func(), &ElaTabWidgetPrivate::onTabDragCreate);
        connect(customTabBar, &ElaTabBar::tabDragDrop, customTabWidget->d_func(), &ElaTabWidgetPrivate::onTabDragDrop);
        floatWidget->addTab(q->widget(index), tabIcon, tabText);
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
        _customTabBar->insertTab(dropIndex, tabIcon, tabText);
    }
}
