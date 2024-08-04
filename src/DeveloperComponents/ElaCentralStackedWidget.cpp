#include "ElaCentralStackedWidget.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
ElaCentralStackedWidget::ElaCentralStackedWidget(QWidget* parent)
    : QStackedWidget(parent)
{
    setObjectName("ElaCentralStackedWidget");

    _windowLinearGradient = new QLinearGradient(0, 0, width(), height());
    _windowLinearGradient->setColorAt(0, ElaThemeColor(eTheme->getThemeMode(), WindowCentralStackBaseStart));
    _windowLinearGradient->setColorAt(1, ElaThemeColor(eTheme->getThemeMode(), WindowCentralStackBaseEnd));

    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, &ElaCentralStackedWidget::onThemeModeChanged);
}

ElaCentralStackedWidget::~ElaCentralStackedWidget()
{
}

void ElaCentralStackedWidget::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
    _windowLinearGradient->setColorAt(0, ElaThemeColor(_themeMode, WindowCentralStackBaseStart));
    _windowLinearGradient->setColorAt(1, ElaThemeColor(_themeMode, WindowCentralStackBaseEnd));
}

void ElaCentralStackedWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect targetRect = this->rect();
    targetRect.adjust(1, 1, 10, 10);
    _windowLinearGradient->setFinalStop(rect().bottomRight());
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(ElaThemeColor(_themeMode, WindowBaseLine), 1.5));
    painter.setBrush(*_windowLinearGradient);
    painter.drawRoundedRect(targetRect, 10, 10);
    painter.restore();
    QStackedWidget::paintEvent(event);
}
