#include "ElaDrawerContainer.h"

#include "ElaTheme.h"

#include <QPainter>
#include <QPropertyAnimation>
ElaDrawerContainer::ElaDrawerContainer(QWidget* parent)
    : QWidget(parent)
{
    _pBorderRadius = 6;
    setObjectName("ElaDrawerContainer");
    setStyleSheet("#ElaDrawerContainer{background-color:transparent;}");

    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 0, 0, 0);

    _containerWidget = new QWidget(this);
    _containerWidget->setObjectName("ElaDrawerContainerWidget");
    _containerWidget->setStyleSheet("#ElaDrawerContainerWidget{background-color:transparent;}");
    setMaximumHeight(0);

    _containerLayout = new QVBoxLayout(_containerWidget);
    _containerLayout->setContentsMargins(0, 0, 0, 0);
    _containerLayout->setSpacing(0);

    _mainLayout->addWidget(_containerWidget);

    _opacityEffect = new QGraphicsOpacityEffect(this);
    _opacityEffect->setOpacity(0);
    setGraphicsEffect(_opacityEffect);

    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaDrawerContainer::~ElaDrawerContainer()
{
}

void ElaDrawerContainer::addWidget(QWidget* widget)
{
    if (!widget || _drawerWidgetList.contains(widget))
    {
        return;
    }
    _containerLayout->addWidget(widget);
    _drawerWidgetList.append(widget);
}

void ElaDrawerContainer::removeWidget(QWidget* widget)
{
    if (!widget)
    {
        return;
    }
    _containerLayout->removeWidget(widget);
    _drawerWidgetList.removeOne(widget);
}

void ElaDrawerContainer::doDrawerAnimation(bool isExpand)
{
    if (_containerLayout->count() == 0)
    {
        return;
    }
    QPropertyAnimation* heightAnimation = new QPropertyAnimation(this, "maximumHeight");
    connect(heightAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        setMinimumHeight(value.toUInt());
    });
    heightAnimation->setEasingCurve(QEasingCurve::OutCubic);
    heightAnimation->setDuration(isExpand ? 300 : 450);
    heightAnimation->setStartValue(maximumHeight());
    heightAnimation->setEndValue(isExpand ? 200 : 0);
    heightAnimation->start(QPropertyAnimation::DeleteWhenStopped);

    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(_opacityEffect, "opacity");
    opacityAnimation->setEasingCurve(isExpand ? QEasingCurve::InCubic : QEasingCurve::OutCubic);
    opacityAnimation->setDuration(isExpand ? 300 : 0);
    opacityAnimation->setStartValue(_opacityEffect->opacity());
    opacityAnimation->setEndValue(isExpand ? 1 : 0);
    opacityAnimation->start(QPropertyAnimation::DeleteWhenStopped);
}

void ElaDrawerContainer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    // 背景绘制
    painter.setPen(ElaThemeColor(_themeMode, BasicBorder));
    painter.setBrush(ElaThemeColor(_themeMode, BasicBaseAlpha));
    QRect foregroundRect(1, 1 - 2 * _pBorderRadius, width() - 2, height() - 2 + 2 * _pBorderRadius);
    painter.drawRoundedRect(foregroundRect, _pBorderRadius, _pBorderRadius);
    // 分割线绘制
    int drawerHeight = 0;
    for (int i = 0; i < _drawerWidgetList.count() - 1; i++)
    {
        QWidget* drawerWidget = _drawerWidgetList[i];
        drawerHeight += drawerWidget->height();
        painter.drawLine(0, drawerHeight, width(), drawerHeight);
    }
    painter.restore();
}
