#include "ElaScrollPagePrivate.h"

#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <QTimer>

#include "ElaBreadcrumbBar.h"
#include "ElaScrollPage.h"

ElaScrollPagePrivate::ElaScrollPagePrivate(QObject* parent)
    : QObject{parent}
{
}

ElaScrollPagePrivate::~ElaScrollPagePrivate()
{
}

void ElaScrollPagePrivate::onNavigationRoute(QVariantMap routeData)
{
    // 面包屑
    Q_Q(ElaScrollPage);
    QString pageCheckSumKey = routeData.value("ElaScrollPageCheckSumKey").toString();
    bool isRouteBack = routeData.value("ElaRouteBackMode").toBool();
    if (pageCheckSumKey == "Navigation")
    {
        QString pageTitle = isRouteBack ? routeData.value("ElaBackPageTitle").toString() : routeData.value("ElaForwardPageTitle").toString();
        q->navigation(_centralWidgetMap.value(pageTitle), false);
    }
    else if (pageCheckSumKey == "BreadcrumbClicked")
    {
        QStringList breadcrumbList = isRouteBack ? routeData.value("ElaBackBreadcrumbList").toStringList() : routeData.value("ElaForwardBreadcrumbList").toStringList();
        int widgetIndex = _centralWidgetMap.value(breadcrumbList.last());
        _switchCentralStackIndex(widgetIndex, _navigationTargetIndex);
        _navigationTargetIndex = widgetIndex;
        _breadcrumbBar->setBreadcrumbList(breadcrumbList);
    }
}

void ElaScrollPagePrivate::_switchCentralStackIndex(int targetIndex, int lastIndex)
{
    QWidget* currentWidget = _centralStackedWidget->widget(lastIndex);
    QWidget* targetWidget = _centralStackedWidget->widget(targetIndex);
    targetWidget->resize(currentWidget->size());
    targetWidget->setVisible(true);

    QPropertyAnimation* currentWidgetAnimation = new QPropertyAnimation(currentWidget, "pos");
    currentWidgetAnimation->setEasingCurve(QEasingCurve::InExpo);
    currentWidgetAnimation->setDuration(300);

    QPropertyAnimation* targetWidgetAnimation = new QPropertyAnimation(targetWidget, "pos");
    connect(targetWidgetAnimation, &QPropertyAnimation::finished, this, [=]() {
        _centralStackedWidget->setCurrentIndex(targetIndex);
    });
    targetWidgetAnimation->setEasingCurve(QEasingCurve::InExpo);
    targetWidgetAnimation->setDuration(300);
    if (targetIndex > lastIndex)
    {
        //左滑
        currentWidgetAnimation->setStartValue(currentWidget->pos());
        currentWidgetAnimation->setEndValue(QPoint(-_centralStackedWidget->width(), 0));
        targetWidgetAnimation->setStartValue(QPoint(_centralStackedWidget->width(), 0));
        targetWidgetAnimation->setEndValue(QPoint(0, 0));
    }
    else
    {
        //右滑
        currentWidgetAnimation->setStartValue(currentWidget->pos());
        currentWidgetAnimation->setEndValue(QPoint(_centralStackedWidget->width(), 0));
        targetWidgetAnimation->setStartValue(QPoint(-_centralStackedWidget->width(), 0));
        targetWidgetAnimation->setEndValue(QPoint(0, 0));
    }
    currentWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    targetWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
