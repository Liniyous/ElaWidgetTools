#include "ElaPushButton.h"

#include "ElaApplication.h"
#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaPushButtonPrivate.h"
ELA_PROPERTY_Q_CPP(ElaPushButton, int, BorderRadius)
ELA_PROPERTY_Q_CPP(ElaPushButton, QColor, LightDefaultColor)
ELA_PROPERTY_Q_CPP(ElaPushButton, QColor, DarkDefaultColor)
ELA_PROPERTY_Q_CPP(ElaPushButton, QColor, LightHoverColor)
ELA_PROPERTY_Q_CPP(ElaPushButton, QColor, DarkHoverColor)
ELA_PROPERTY_Q_CPP(ElaPushButton, QColor, LightPressColor)
ELA_PROPERTY_Q_CPP(ElaPushButton, QColor, DarkPressColor)
ElaPushButton::ElaPushButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaPushButtonPrivate())
{
    Q_D(ElaPushButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = eTheme->getThemeMode();
    d->_pLightDefaultColor = ElaThemeColor(ElaThemeType::Light, BasicBase);
    d->_pDarkDefaultColor = ElaThemeColor(ElaThemeType::Dark, BasicBase);
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, BasicHover);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, BasicHover);
    d->_pLightPressColor = ElaThemeColor(ElaThemeType::Light, BasicPress);
    d->_pDarkPressColor = ElaThemeColor(ElaThemeType::Dark, BasicPress);
    d->_lightTextColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_darkTextColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    setMouseTracking(true);
    setFixedHeight(30);
    QFont font = this->font();
    font.setPixelSize(eApp->getFontPixelSize() + 2);
    setFont(font);
    setObjectName("ElaPushButton");
    setStyleSheet("#ElaPushButton{background-color:transparent;}");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaPushButton::ElaPushButton(const QString& text, QWidget* parent)
    : ElaPushButton(parent)
{
    setText(text);
}

ElaPushButton::~ElaPushButton()
{
}

void ElaPushButton::setLightTextColor(const QColor& color)
{
    Q_D(ElaPushButton);
    d->_lightTextColor = color;
}

const QColor& ElaPushButton::getLightTextColor() const
{
    Q_D(const ElaPushButton);
    return d->_lightTextColor;
}

void ElaPushButton::setDarkTextColor(const QColor& color)
{
    Q_D(ElaPushButton);
    d->_darkTextColor = color;
}

const QColor& ElaPushButton::getDarkTextColor() const
{
    Q_D(const ElaPushButton);
    return d->_darkTextColor;
}

void ElaPushButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaPushButton);
    d->_isPressed = true;
    QPushButton::mousePressEvent(event);
}

void ElaPushButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaPushButton);
    d->_isPressed = false;
    QPushButton::mouseReleaseEvent(event);
}

void ElaPushButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaPushButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

    // 背景绘制
    painter.save();
    QRect foregroundRect = rect().adjusted(1, 1, -1, -1);
    if (d->_themeMode == ElaThemeType::Light)
    {
        painter.setPen(ElaThemeColor(ElaThemeType::Light, BasicBorder));
        painter.setBrush(isEnabled() ? d->_isPressed ? d->_pLightPressColor : (underMouse() ? d->_pLightHoverColor : d->_pLightDefaultColor) : ElaThemeColor(d->_themeMode, BasicDisable));
    }
    else
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(isEnabled() ? d->_isPressed ? d->_pDarkPressColor : (underMouse() ? d->_pDarkHoverColor : d->_pDarkDefaultColor) : ElaThemeColor(d->_themeMode, BasicDisable));
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 底边线绘制
    if (!d->_isPressed)
    {
        painter.setPen(ElaThemeColor(d->_themeMode, BasicBaseLine));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - 1, foregroundRect.width() - d->_pBorderRadius, height() - 1);
    }
    //文字绘制
    painter.setPen(isEnabled() ? d->_themeMode == ElaThemeType::Light ? d->_lightTextColor : d->_darkTextColor : ElaThemeColor(d->_themeMode, BasicTextDisable));
    painter.drawText(foregroundRect, Qt::AlignCenter, text());
    painter.restore();
}
