#include "ElaScrollPageArea.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>

#include "ElaApplication.h"
#include "private/ElaScrollPageAreaPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaScrollPageArea, int, BorderRadius)
ElaScrollPageArea::ElaScrollPageArea(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaScrollPageAreaPrivate())
{
    Q_D(ElaScrollPageArea);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    setMinimumSize(300, 80);
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaScrollPageArea::~ElaScrollPageArea()
{
}

void ElaScrollPageArea::paintEvent(QPaintEvent* event)
{
    Q_D(ElaScrollPageArea);
    QPainter painter(this);
    painter.save();
    if (d->_themeMode == ElaApplicationType::Light)
    {
        painter.setPen(QPen(QColor(0xDF, 0xDF, 0xDF), 1));
        painter.setBrush(QColor(0xFB, 0xFB, 0xFD));
    }
    else
    {
        painter.setPen(QPen(QColor(0x37, 0x37, 0x37), 1));
        painter.setBrush(QColor(0x26, 0x2C, 0x36));
    }
    QRect foregroundRect(1, 1, width() - 2, height() - 2);
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.restore();
}
