#include "ElaDrawerContainer.h"

#include "ElaTheme.h"

#include <QPainter>
#include <QPropertyAnimation>
ElaDrawerContainer::ElaDrawerContainer(QWidget* parent)
    : QWidget(parent)
{
    _pBorderRadius = 6;
    _pOpacity = 0;
    _pContainerPix = QPixmap();
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
    _containerWidget->setVisible(true);
    _isShowBorder = true;
    if (isExpand)
    {
        setFixedHeight(_calculateContainerMinimumHeight());
    }
    _pContainerPix = grab(rect());
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "pOpacity");
    connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() {
        _pContainerPix = QPixmap();
        if (isExpand)
        {
            _containerWidget->setVisible(true);
            _isShowBorder = true;
        }
        else
        {
            setFixedHeight(0);
        }
    });
    opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
    opacityAnimation->setDuration(300);
    opacityAnimation->setStartValue(_pOpacity);
    opacityAnimation->setEndValue(isExpand ? 1 : 0);
    opacityAnimation->start(QPropertyAnimation::DeleteWhenStopped);
    _containerWidget->setVisible(false);
    _isShowBorder = false;
}

void ElaDrawerContainer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    if (_isShowBorder)
    {
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
    }
    if (!_pContainerPix.isNull())
    {
        painter.setOpacity(_pOpacity);
        painter.drawPixmap(QRect(0, -height() * (1 - _pOpacity), width(), height()), _pContainerPix);
    }
    painter.restore();
}

int ElaDrawerContainer::_calculateContainerMinimumHeight() const
{
    int minimumHeight = 0;
    for (auto widget: _drawerWidgetList)
    {
        minimumHeight += widget->minimumHeight();
    }
    minimumHeight = std::max(100, minimumHeight);
    return minimumHeight;
}
