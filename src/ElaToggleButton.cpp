#include "ElaToggleButton.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaApplication.h"
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
    eApp->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    painter.save();
    QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * (d->_shadowBorderWidth), height() - 2 * d->_shadowBorderWidth);
    if (d->_themeMode == ElaApplicationType::Light)
    {
        if (d->_isToggled)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(d->_isPressed ? QColor(0x00, 0x7A, 0xD8) : (underMouse() ? QColor(0x00, 0x70, 0xC6) : QColor(0x00, 0x66, 0xB9)));
        }
        else
        {
            painter.setPen(QPen(QColor(0xDF, 0xDF, 0xDF), 2));
            painter.setBrush(d->_isPressed ? QColor(0xF2, 0xF2, 0xF2) : (underMouse() ? QColor(0xF6, 0xF6, 0xF6) : QColor(0xFE, 0xFE, 0xFE)));
        }
    }
    else
    {
        if (d->_isToggled)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(0x4C, 0xA0, 0xE0));
        }
        else
        {
            painter.setPen(QPen(QColor(0x50, 0x50, 0x50), 2));
            painter.setBrush((underMouse() ? QColor(0x44, 0x44, 0x44) : QColor(0x3E, 0x3E, 0x3E)));
        }
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 底边线绘制
    if (!d->_isPressed)
    {
        painter.setPen(QPen(QColor(0xBC, 0xBC, 0xBC), 1));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - d->_shadowBorderWidth, foregroundRect.width(), height() - d->_shadowBorderWidth);
    }

    //文字绘制
    painter.setPen(d->_themeMode == ElaApplicationType::Light ? (d->_isToggled ? QColor(0xFD, 0xFD, 0xFD) : QColor(0x45, 0x44, 0x41)) : d->_isToggled ? QColor(0x01, 0x01, 0x02)
                                                                                                                                                      : QColor(0xFE, 0xFE, 0xFE));
    painter.drawText(foregroundRect, Qt::AlignCenter, text());
    painter.restore();
}
