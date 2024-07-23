#include "ElaToggleButton.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaToggleButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaToggleButton, int, BorderRadius)
ElaToggleButton::ElaToggleButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaToggleButtonPrivate())
{
    Q_D(ElaToggleButton);
    d->q_ptr = this;
    d->_initStyle();
}

ElaToggleButton::ElaToggleButton(QString text, QWidget* parent)
    : ElaToggleButton(parent)
{
    setText(text);
}

ElaToggleButton::~ElaToggleButton()
{
}

void ElaToggleButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaToggleButton);
    d->_isPressed = true;
    QPushButton::mouseReleaseEvent(event);
}

void ElaToggleButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaToggleButton);
    d->_isPressed = false;
    d->_isToggled = !d->_isToggled;
    Q_EMIT this->toggled(d->_isToggled);
    QPushButton::mouseReleaseEvent(event);
}

void ElaToggleButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaToggleButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    painter.save();
    QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * (d->_shadowBorderWidth), height() - 2 * d->_shadowBorderWidth);
    if (d->_isToggled)
    {
        painter.setPen(ElaThemeColor(d->_themeMode, ToggleButtonToggledBorder));
        painter.setBrush(d->_isPressed ? ElaThemeColor(d->_themeMode, ToggleButtonToggledPress) : (underMouse() ? ElaThemeColor(d->_themeMode, ToggleButtonToggledHover) : ElaThemeColor(d->_themeMode, ToggleButtonToggledBase)));
    }
    else
    {
        painter.setPen(QPen(ElaThemeColor(d->_themeMode, ToggleButtonNoToggledBorder), 2));
        painter.setBrush(d->_isPressed ? ElaThemeColor(d->_themeMode, ToggleButtonNoToggledPress) : (underMouse() ? ElaThemeColor(d->_themeMode, ToggleButtonNoToggledHover) : ElaThemeColor(d->_themeMode, ToggleButtonNoToggledBase)));
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 底边线绘制
    if (!d->_isPressed)
    {
        painter.setPen(QPen(ElaThemeColor(d->_themeMode, ToggleButtonHemline), 1));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - d->_shadowBorderWidth, foregroundRect.width(), height() - d->_shadowBorderWidth);
    }

    //文字绘制
    painter.setPen((d->_isToggled ? ElaThemeColor(d->_themeMode, ToggleButtonToggledText) : ElaThemeColor(d->_themeMode, ToggleButtonNoToggledText)));
    painter.drawText(foregroundRect, Qt::AlignCenter, text());
    painter.restore();
}
