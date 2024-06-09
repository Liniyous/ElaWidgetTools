#include "ElaScrollPageArea.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>

#include "ElaApplication.h"
ElaScrollPageArea::ElaScrollPageArea(QWidget* parent)
    : QWidget{parent}
{
    _pBorderRadius = 6;
    setMinimumSize(300, 80);
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode)
            { _themeMode = themeMode; });
}

ElaScrollPageArea::~ElaScrollPageArea()
{
}

void ElaScrollPageArea::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    if (_themeMode == ElaApplicationType::Light)
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
    painter.drawRoundedRect(foregroundRect, _pBorderRadius, _pBorderRadius);
    painter.restore();
}
