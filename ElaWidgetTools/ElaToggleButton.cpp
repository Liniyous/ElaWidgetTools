#include "ElaToggleButton.h"

#include <QEvent>
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
    setFixedSize(80, 32);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("ElaToggleButton");
    setStyleSheet("#ElaToggleButton{background-color:transparent;}");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
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
    d->_pToggleAlpha = isToggled ? 255 : 0;
    update();
    Q_EMIT toggled(isToggled);
}

bool ElaToggleButton::getIsToggled() const
{
    Q_D(const ElaToggleButton);
    return d->_isToggled;
}

bool ElaToggleButton::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Enter:
    case QEvent::Leave:
    {
        update();
        break;
    }
    default:
    {
        break;
    }
    }
    return QWidget::event(event);
}

void ElaToggleButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaToggleButton);
    d->_isPressed = true;
    update();
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
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter.save();
    QRect foregroundRect(1, 1, width() - 2, height() - 2);
    if (d->_isAlphaAnimationFinished)
    {
        if (d->_isToggled)
        {
            painter.setPen(ElaThemeColor(d->_themeMode, BasicBorder));
            painter.setBrush(isEnabled() ? d->_isPressed ? ElaThemeColor(d->_themeMode, PrimaryPress) : (underMouse() ? ElaThemeColor(d->_themeMode, PrimaryHover) : ElaThemeColor(d->_themeMode, PrimaryNormal)) : ElaThemeColor(d->_themeMode, BasicDisable));
        }
        else
        {
            painter.setPen(ElaThemeColor(d->_themeMode, BasicBorder));
            painter.setBrush(isEnabled() ? d->_isPressed ? ElaThemeColor(d->_themeMode, BasicPress) : (underMouse() ? ElaThemeColor(d->_themeMode, BasicHover) : ElaThemeColor(d->_themeMode, BasicBase)) : ElaThemeColor(d->_themeMode, BasicDisable));
        }
    }
    else
    {
        painter.setPen(Qt::NoPen);
        QColor toggleColor = ElaThemeColor(d->_themeMode, PrimaryNormal);
        toggleColor.setAlpha(d->_pToggleAlpha);
        painter.setBrush(toggleColor);
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 底边线绘制
    if (!d->_isPressed && !d->_isToggled)
    {
        painter.setPen(ElaThemeColor(d->_themeMode, BasicBaseLine));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - 1, foregroundRect.x() + foregroundRect.width() - d->_pBorderRadius, height() - 1);
    }

    //文字绘制
    painter.setPen(isEnabled() ? d->_isToggled ? ElaThemeColor(d->_themeMode, BasicTextInvert) : ElaThemeColor(d->_themeMode, BasicText) : ElaThemeColor(d->_themeMode, BasicTextDisable));
    painter.drawText(foregroundRect, Qt::AlignCenter, d->_pText);
    painter.restore();
}
