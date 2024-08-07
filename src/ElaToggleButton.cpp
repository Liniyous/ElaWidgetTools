#include "ElaToggleButton.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "ElaTheme.h"
#include "private/ElaToggleButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaToggleButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaToggleButton, QString, Text)
ElaToggleButton::ElaToggleButton(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaToggleButtonPrivate())
{
    Q_D(ElaToggleButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = eTheme->getThemeMode();
    d->_pToggleAlpha = 0;
    setMouseTracking(true);
    setFixedSize(80, 38);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("ElaToggleButton");
    setStyleSheet("#ElaToggleButton{background-color:transparent;}");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaToggleButton::ElaToggleButton(QString text, QWidget* parent)
    : ElaToggleButton(parent)
{
    Q_D(ElaToggleButton);
    d->_pText = text;
}

ElaToggleButton::~ElaToggleButton()
{
}

void ElaToggleButton::setIsToggled(bool isToggled)
{
    Q_D(ElaToggleButton);
    d->_isToggled = isToggled;
    update();
    Q_EMIT toggled(isToggled);
}

bool ElaToggleButton::getIsToggled() const
{
    Q_D(const ElaToggleButton);
    return d->_isToggled;
}

void ElaToggleButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaToggleButton);
    d->_isPressed = true;
    QWidget::mouseReleaseEvent(event);
}

void ElaToggleButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaToggleButton);
    d->_isPressed = false;
    d->_isAlphaAnimationFinished = false;
    d->_isToggled = !d->_isToggled;
    QPropertyAnimation* alphaAnimation = new QPropertyAnimation(d, "pToggleAlpha");
    connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() {
        d->_isAlphaAnimationFinished = true;
    });
    alphaAnimation->setDuration(250);
    alphaAnimation->setStartValue(d->_pToggleAlpha);
    if (d->_isToggled)
    {
        alphaAnimation->setEndValue(255);
    }
    else
    {
        alphaAnimation->setEndValue(0);
    }
    alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    Q_EMIT this->toggled(d->_isToggled);
    QWidget::mouseReleaseEvent(event);
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
    if (d->_isAlphaAnimationFinished)
    {
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
    }
    else
    {
        painter.setPen(Qt::NoPen);
        QColor toggleColor = ElaThemeColor(d->_themeMode, ToggleButtonToggledBase);
        toggleColor.setAlpha(d->_pToggleAlpha);
        painter.setBrush(toggleColor);
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
    painter.drawText(foregroundRect, Qt::AlignCenter, d->_pText);
    painter.restore();
}
