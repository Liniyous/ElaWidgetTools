#include "ElaToggleSwitch.h"

#include "ElaColorAnimation.h"
#include "ElaControlStyle.h"
#include "ElaTheme.h"
#include "private/ElaToggleSwitchPrivate.h"

#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStyleOption>

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
    if (event->button() != Qt::LeftButton)
    {
        QWidget::mousePressEvent(event);
        return;
    }
    Q_D(ElaToggleSwitch);
    if (d->_positionAnimation)
    {
        d->_positionAnimation->stop();
    }
    d->_adjustCircleCenterX();
    setProperty("circleCenterX", d->_circleCenterX);
    d->_isLeftButtonPress = true;
    d->_isDragging = false;
    d->_mousePressPos = event->pos();
    d->_lastMouseX = event->pos().x();
    d->_startRadiusAnimation(d->_circleRadius, height() * 0.25);
    event->accept();
}

void ElaToggleSwitch::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaToggleSwitch);
    if (event->button() != Qt::LeftButton || !d->_isLeftButtonPress)
    {
        QWidget::mouseReleaseEvent(event);
        return;
    }
    d->_isLeftButtonPress = false;
    bool isToggled = d->_isToggled;
    if (d->_isDragging)
    {
        isToggled = d->_circleCenterX > width() / 2;
    }
    else if (rect().contains(event->pos()))
    {
        isToggled = !isToggled;
    }
    d->_isDragging = false;
    d->_startPosAnimation(d->_circleCenterX, isToggled ? width() - height() / 2 - d->_margin * 2 : height() / 2, isToggled);
    d->_startRadiusAnimation(height() * 0.25, height() * 0.35);
    event->accept();
}

void ElaToggleSwitch::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaToggleSwitch);
    if (!d->_isLeftButtonPress)
    {
        QWidget::mouseMoveEvent(event);
        return;
    }
    // 滑块立即跟手，距离阈值仅用于松开时区分点击与拖动
    if ((event->pos() - d->_mousePressPos).manhattanLength() >= qMax(1, QApplication::startDragDistance()))
    {
        d->_isDragging = true;
    }
    d->_circleCenterX += event->pos().x() - d->_lastMouseX;
    d->_lastMouseX = event->pos().x();
    d->_adjustCircleCenterX();
    setProperty("circleCenterX", d->_circleCenterX);
    update();
    event->accept();
}

void ElaToggleSwitch::paintEvent(QPaintEvent* event)
{
    Q_D(ElaToggleSwitch);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 背景绘制
    painter.setPen(d->_isToggled ? QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1.5) : QPen(ElaThemeColor(d->_themeMode, BasicBorderDeep), 1.5));
    QStyleOption option;
    option.initFrom(this);
    option.state.setFlag(QStyle::State_Sunken, d->_isLeftButtonPress);
    painter.setBrush(ElaColorAnimation::getColor(this, ElaControlStyle::getIndicatorBackgroundColor(d->_themeMode, option.state, d->_isToggled)));
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
    painter.setBrush(ElaColorAnimation::getColor(this, isEnabled() ? d->_isToggled ? ElaThemeColor(d->_themeMode, BasicTextInvert) : ElaThemeColor(d->_themeMode, BasicIndicator) : ElaThemeColor(d->_themeMode, BasicTextDisable), "Thumb"));
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
