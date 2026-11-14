#include "ElaMicaPainter.h"

#include <QPainter>
#include <QPixmap>

void ElaMicaPainter::drawScreen(QPainter* painter, const QRectF& windowRect, qreal coordinateScale, const QRectF& screenRect, const QPixmap& pixmap)
{
    const QRectF intersection = windowRect.intersected(screenRect);
    if (intersection.isEmpty() || pixmap.isNull())
    {
        return;
    }
    const QRectF targetRect((intersection.topLeft() - windowRect.topLeft()) / coordinateScale, intersection.size() / coordinateScale);
    const qreal xRatio = pixmap.width() / screenRect.width();
    const qreal yRatio = pixmap.height() / screenRect.height();
    const QRectF sourceRect((intersection.x() - screenRect.x()) * xRatio, (intersection.y() - screenRect.y()) * yRatio,
                            intersection.width() * xRatio, intersection.height() * yRatio);
    painter->drawPixmap(targetRect, pixmap, sourceRect);
}
