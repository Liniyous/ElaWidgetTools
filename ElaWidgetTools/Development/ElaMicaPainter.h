#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAMICAPAINTER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAMICAPAINTER_H_

#include <QRectF>

class QPainter;
class QPixmap;
class ElaMicaPainter
{
public:
    static void drawScreen(QPainter* painter, const QRectF& windowRect, qreal coordinateScale, const QRectF& screenRect, const QPixmap& pixmap);
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELAMICAPAINTER_H_
