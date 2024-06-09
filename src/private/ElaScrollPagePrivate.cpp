#include "ElaScrollPagePrivate.h"

#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <QTimer>

ElaScrollPagePrivate::ElaScrollPagePrivate(QObject* parent)
    : QObject{parent}
{
}

ElaScrollPagePrivate::~ElaScrollPagePrivate()
{
}

void ElaScrollPagePrivate::_switchCentralStackIndex(int targetIndex, int lastIndex)
{
    QWidget* currentWidget = _centralStackedWidget->widget(lastIndex);
    QWidget* targetWidget = _centralStackedWidget->widget(targetIndex);
    targetWidget->setVisible(true);

    QPropertyAnimation* currentWidgetAnimation = new QPropertyAnimation(currentWidget, "pos");
    currentWidgetAnimation->setEasingCurve(QEasingCurve::InOutSine);
    currentWidgetAnimation->setDuration(280);

    QPropertyAnimation* targetWidgetAnimation = new QPropertyAnimation(targetWidget, "pos");
    connect(targetWidgetAnimation, &QPropertyAnimation::finished, this, [=]() {
        _centralStackedWidget->setCurrentIndex(targetIndex);
    });
    targetWidgetAnimation->setEasingCurve(QEasingCurve::InOutSine);
    targetWidgetAnimation->setDuration(280);
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
