#include "ElaCentralStackedWidget.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
ElaCentralStackedWidget::ElaCentralStackedWidget(QWidget* parent)
    : QStackedWidget(parent)
{
    setObjectName("ElaCentralStackedWidget");
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, &ElaCentralStackedWidget::onThemeModeChanged);
}

ElaCentralStackedWidget::~ElaCentralStackedWidget()
{
}

void ElaCentralStackedWidget::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
}

void ElaCentralStackedWidget::setIsTransparent(bool isTransparent)
{
    this->_isTransparent = isTransparent;
    update();
}

bool ElaCentralStackedWidget::getIsTransparent() const
{
    return _isTransparent;
}

void ElaCentralStackedWidget::setIsHasRadius(bool isHasRadius)
{
    this->_isHasRadius = isHasRadius;
    update();
}

void ElaCentralStackedWidget::paintEvent(QPaintEvent* event)
{
    if (!_isTransparent)
    {
        QPainter painter(this);
        QRect targetRect = this->rect();
        targetRect.adjust(1, 1, 10, 10);
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(ElaThemeColor(_themeMode, BasicPageLine)));
        painter.setBrush(ElaThemeColor(_themeMode, WindowCentralStackBase));
        if (_isHasRadius)
        {
            painter.drawRoundedRect(targetRect, 10, 10);
        }
        else
        {
            painter.drawRect(targetRect);
        }
        painter.restore();
    }
    QStackedWidget::paintEvent(event);
}
