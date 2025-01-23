#include "ElaColorPreview.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
ElaColorPreview::ElaColorPreview(QWidget* parent)
    : QWidget{parent}
{
    setFixedSize(50, 260);
    _pThemeMode = eTheme->getThemeMode();
}

ElaColorPreview::~ElaColorPreview()
{
}

void ElaColorPreview::paintEvent(QPaintEvent* event)
{
    int penWidth = 3;
    int borderRadius = 5;
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    QRect viewRect = rect();
    viewRect.adjust(penWidth, penWidth, -penWidth, -penWidth);
    painter.setPen(QPen(ElaThemeColor(_pThemeMode, BasicBorder), penWidth));
    painter.setBrush(_pBaseColor);
    painter.drawRoundedRect(viewRect, borderRadius, borderRadius);
    painter.restore();
}
