#include "ElaToggleSwitch.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaApplication.h"
#include "private/ElaToggleSwitchPrivate.h"
ElaToggleSwitch::ElaToggleSwitch(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaToggleSwitchPrivate())
{
    Q_D(ElaToggleSwitch);
    d->q_ptr = this;
    setMouseTracking(true);
    setFixedSize(44, 22);
    d->_circleCenterX = 0;
    d->_isToggled = false;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    setProperty("circleCenterX", 0.01);
    setProperty("circleRadius", 0.01);
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode)
            { d->_themeMode = themeMode; });
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
        d->_startRadiusAnimation(height() * 0.3, height() * 0.35);
        break;
    }
    case QEvent::Leave:
    {
        d->_startRadiusAnimation(height() * 0.35, height() * 0.3);
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
        if (d->_isAnimationFinished)
        {
            if (d->_isToggled)
            {
                d->_startPosAnimation(width() - height() / 2 - d->_margin * 2, height() / 2, false);
            }
            else
            {
                d->_startPosAnimation(height() / 2, width() - height() / 2 - d->_margin * 2, true);
            }
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
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 背景绘制
    if (d->_themeMode == ElaApplicationType::Light)
    {
        painter.setPen(d->_isToggled ? QPen(QColor(0x00, 0x64, 0xB0), 2) : QPen(QColor(0xA5, 0xA5, 0xA7), 2));
        painter.setBrush(d->_isToggled ? QColor(0x00, 0x64, 0xB0) : (underMouse() ? QColor(0xF0, 0xF0, 0xF0) : QColor(0xFD, 0xFD, 0xFD)));
    }
    else
    {
        painter.setPen(d->_isToggled ? Qt::NoPen : QPen(QColor(0xA1, 0xA1, 0xA1), 2));
        painter.setBrush(d->_isToggled ? QColor(0x4C, 0xA0, 0xE0) : (underMouse() ? QColor(0x3E, 0x3E, 0x3E) : QColor(0x32, 0x32, 0x32)));
    }
    QPainterPath path;
    path.moveTo(width() - height() - d->_margin, height() - d->_margin);
    path.arcTo(QRectF(QPointF(width() - height() - d->_margin, d->_margin), QSize(height() - d->_margin * 2, height() - d->_margin * 2)), -90, 180);
    path.lineTo(height() / 2 + d->_margin, d->_margin);
    path.arcTo(QRectF(QPointF(d->_margin, d->_margin), QSize(height() - d->_margin * 2, height() - d->_margin * 2)), 90, 180);
    path.lineTo(width() - height() - d->_margin, height() - d->_margin);
    // path.closeSubpath();
    painter.drawPath(path);

    // 圆心绘制
    if (d->_themeMode == ElaApplicationType::Light)
    {
        painter.setBrush(d->_isToggled ? QColor(0xFD, 0xFF, 0xF3) : QColor(0x5D, 0x5D, 0x60));
    }
    else
    {
        painter.setBrush(d->_isToggled ? QColor(0x00, 0x00, 0x00) : QColor(0xD0, 0xD0, 0xD0));
    }
    painter.setPen(Qt::NoPen);
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
        if (d->_isAnimationFinished)
        {
            painter.drawEllipse(QPointF(d->_isToggled ? width() - height() / 2 - d->_margin * 2 : height() / 2, height() / 2), d->_circleRadius, d->_circleRadius);
        }
        else
        {
            painter.drawEllipse(QPointF(d->_circleCenterX, height() / 2), d->_circleRadius, d->_circleRadius);
        }
    }
    painter.restore();
}
