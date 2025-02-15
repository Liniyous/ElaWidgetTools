#include "ElaToggleSwitch.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaToggleSwitchPrivate.h"
ElaToggleSwitch::ElaToggleSwitch(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaToggleSwitchPrivate())
{
    Q_D(ElaToggleSwitch);
    d->q_ptr = this;
    setObjectName("ElaToggleSwitch");
    setMouseTracking(true);
    setFixedSize(44, 22);
    d->_circleCenterX = -1;
    d->_isToggled = false;
    d->_themeMode = eTheme->getThemeMode();
    setProperty("circleCenterX", 0.01);
    setProperty("circleRadius", 0.01);
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaToggleSwitch::~ElaToggleSwitch()
{
}

void ElaToggleSwitch::setIsToggled(bool isToggled)
{
    Q_D(ElaToggleSwitch);
    if (d->_isToggled == isToggled)
    {
        return;
    }
    if (d->_isToggled)
    {
        d->_startPosAnimation(width() - height() / 2 - d->_margin * 2, height() / 2, isToggled);
    }
    else
    {
        d->_startPosAnimation(height() / 2, width() - height() / 2 - d->_margin * 2, isToggled);
    }
}

bool ElaToggleSwitch::getIsToggled() const
{
    return d_ptr->_isToggled;
}

bool ElaToggleSwitch::event(QEvent* event)
{
    Q_D(ElaToggleSwitch);
    switch (event->type())
    {
    case QEvent::Enter:
    {
        if (isEnabled())
        {
            d->_startRadiusAnimation(height() * 0.3, height() * 0.35);
        }
        break;
    }
    case QEvent::Leave:
    {
        if (isEnabled())
        {
            d->_startRadiusAnimation(height() * 0.35, height() * 0.3);
        }
        break;
    }
    case QEvent::MouseMove:
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

void ElaToggleSwitch::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaToggleSwitch);
    d->_adjustCircleCenterX();
    d->_isLeftButtonPress = true;
    d->_lastMouseX = event->pos().x();
    d->_startRadiusAnimation(d->_circleRadius, height() * 0.25);
    QWidget::mousePressEvent(event);
}

void ElaToggleSwitch::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaToggleSwitch);
    d->_isLeftButtonPress = false;
    QWidget::mouseReleaseEvent(event);
    if (d->_isMousePressMove)
    {
        d->_isMousePressMove = false;
        if (d->_circleCenterX > width() / 2)
        {
            d->_startPosAnimation(d->_circleCenterX, width() - height() / 2 - d->_margin * 2, true);
        }
        else
        {
            d->_startPosAnimation(d->_circleCenterX, height() / 2, false);
        }
    }
    else
    {
        if (d->_isToggled)
        {
            d->_startPosAnimation(d->_circleCenterX, height() / 2, false);
        }
        else
        {
            d->_startPosAnimation(d->_circleCenterX, width() - height() / 2 - d->_margin * 2, true);
        }
    }
    d->_startRadiusAnimation(height() * 0.25, height() * 0.35);
}

void ElaToggleSwitch::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaToggleSwitch);
    if (d->_isLeftButtonPress)
    {
        d->_isMousePressMove = true;
        int moveX = event->pos().x() - d->_lastMouseX;
        d->_lastMouseX = event->pos().x();
        d->_circleCenterX += moveX;
        d->_adjustCircleCenterX();
    }
    QWidget::mouseMoveEvent(event);
}

void ElaToggleSwitch::paintEvent(QPaintEvent* event)
{
    Q_D(ElaToggleSwitch);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 背景绘制
    painter.setPen(d->_isToggled ? QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1.5) : QPen(ElaThemeColor(d->_themeMode, BasicBorderDeep), 1.5));
    painter.setBrush(isEnabled() ? d->_isToggled ? ElaThemeColor(d->_themeMode, PrimaryNormal) : (underMouse() ? ElaThemeColor(d->_themeMode, BasicHover) : ElaThemeColor(d->_themeMode, BasicBase)) : ElaThemeColor(d->_themeMode, BasicDisable));
    QPainterPath path;
    path.moveTo(width() - height() - d->_margin, height() - d->_margin);
    path.arcTo(QRectF(QPointF(width() - height() - d->_margin, d->_margin), QSize(height() - d->_margin * 2, height() - d->_margin * 2)), -90, 180);
    path.lineTo(height() / 2 + d->_margin, d->_margin);
    path.arcTo(QRectF(QPointF(d->_margin, d->_margin), QSize(height() - d->_margin * 2, height() - d->_margin * 2)), 90, 180);
    path.lineTo(width() - height() - d->_margin, height() - d->_margin);
    path.closeSubpath();
    painter.drawPath(path);

    // 圆心绘制
    painter.setPen(Qt::NoPen);
    painter.setBrush(isEnabled() ? d->_isToggled ? ElaThemeColor(d->_themeMode, BasicTextInvert) : ElaThemeColor(d->_themeMode, ToggleSwitchNoToggledCenter) : ElaThemeColor(d->_themeMode, BasicTextDisable));
    if (d->_circleRadius == 0)
    {
        d->_circleRadius = this->isEnabled() ? (underMouse() ? height() * 0.35 : height() * 0.3) : height() * 0.3;
    }
    if (d->_isLeftButtonPress)
    {
        painter.drawEllipse(QPointF(d->_circleCenterX, height() / 2), d->_circleRadius, d->_circleRadius);
    }
    else
    {
        if (d->_circleCenterX == -1)
        {
            d->_circleCenterX = d->_isToggled ? width() - height() / 2 - d->_margin * 2 : height() / 2;
        }
        painter.drawEllipse(QPointF(d->_circleCenterX, height() / 2), d->_circleRadius, d->_circleRadius);
    }
    painter.restore();
}
