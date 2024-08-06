#include "ElaPushButton.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaPushButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, LightDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, DarkDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, LightHoverColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, DarkHoverColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, LightPressColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, DarkPressColor)
ElaPushButton::ElaPushButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaPushButtonPrivate())
{
    Q_D(ElaPushButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = eTheme->getThemeMode();
    d->_pLightDefaultColor = ElaThemeColor(ElaThemeType::Light, PushButtonDefaultBase);
    d->_pDarkDefaultColor = ElaThemeColor(ElaThemeType::Dark, PushButtonDefaultBase);
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, PushButtonDefaultHover);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, PushButtonDefaultHover);
    d->_pLightPressColor = ElaThemeColor(ElaThemeType::Light, PushButtonDefaultPress);
    d->_pDarkPressColor = ElaThemeColor(ElaThemeType::Dark, PushButtonDefaultPress);
    d->_lightTextColor = ElaThemeColor(ElaThemeType::Light, PushButtonDefaultText);
    d->_darkTextColor = ElaThemeColor(ElaThemeType::Dark, PushButtonDefaultText);
    setMouseTracking(true);
    setFixedHeight(38);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("ElaPushButton");
    setStyleSheet("#ElaPushButton{background-color:transparent;}");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaPushButton::ElaPushButton(QString text, QWidget* parent)
    : ElaPushButton(parent)
{
    setText(text);
}

ElaPushButton::~ElaPushButton()
{
}

void ElaPushButton::setLightTextColor(QColor color)
{
    Q_D(ElaPushButton);
    d->_lightTextColor = color;
}

QColor ElaPushButton::getLightTextColor() const
{
    Q_D(const ElaPushButton);
    return d->_lightTextColor;
}

void ElaPushButton::setDarkTextColor(QColor color)
{
    Q_D(ElaPushButton);
    d->_darkTextColor = color;
}

QColor ElaPushButton::getDarkTextColor() const
{
    Q_D(const ElaPushButton);
    return d->_darkTextColor;
}

void ElaPushButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaPushButton);
    d->_isPressed = true;
    QPushButton::mouseReleaseEvent(event);
}

void ElaPushButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaPushButton);
    d->_isPressed = false;
    Q_EMIT clicked();
    QPushButton::mouseReleaseEvent(event);
}

void ElaPushButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaPushButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    // 背景绘制
    painter.save();
    QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * (d->_shadowBorderWidth), height() - 2 * d->_shadowBorderWidth);
    if (d->_themeMode == ElaThemeType::Light)
    {
        painter.setPen(ElaThemeColor(ElaThemeType::Light, PushButtonBorder));
        painter.setBrush(d->_isPressed ? d->_pLightPressColor : (underMouse() ? d->_pLightHoverColor : d->_pLightDefaultColor));
    }
    else
    {
        painter.setPen(ElaThemeColor(ElaThemeType::Dark, PushButtonBorder));
        painter.setBrush(d->_isPressed ? d->_pDarkPressColor : (underMouse() ? d->_pDarkHoverColor : d->_pDarkDefaultColor));
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 底边线绘制
    if (!d->_isPressed)
    {
        painter.setPen(ElaThemeColor(ElaThemeType::Light, PushButtonHemline));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - d->_shadowBorderWidth, foregroundRect.width(), height() - d->_shadowBorderWidth);
    }
    //文字绘制
    painter.setPen(d->_themeMode == ElaThemeType::Light ? d->_lightTextColor : d->_darkTextColor);
    painter.drawText(foregroundRect, Qt::AlignCenter, text());
    painter.restore();
}
