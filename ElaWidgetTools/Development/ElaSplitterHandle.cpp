#include "ElaSplitterHandle.h"

#include "ElaColorAnimation.h"
#include "ElaTheme.h"

#include <QPainter>

ElaSplitterHandle::ElaSplitterHandle(Qt::Orientation orientation, QSplitter* parent)
    : QSplitterHandle(orientation, parent)
{
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this]() {
        update();
    });
}

void ElaSplitterHandle::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    const auto color = ElaColorAnimation::getColor(this, ElaThemeColor(eTheme->getThemeMode(), BasicBaseLine));
    if (orientation() == Qt::Horizontal)
    {
        painter.fillRect(QRect(width() / 2, 0, 1, height()), color);
    }
    else
    {
        painter.fillRect(QRect(0, height() / 2, width(), 1), color);
    }
}
