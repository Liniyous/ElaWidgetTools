#include "ElaImageCard.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>

ElaImageCard::ElaImageCard(QWidget* parent)
    : QWidget(parent)
{
    _pBorderRadius = 6;
    _pMaximumAspectRatio = 2.2;
    setMinimumSize(350, 260);
    _pIsPreserveAspectCrop = true;
}

ElaImageCard::~ElaImageCard()
{
}

void ElaImageCard::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect(), _pBorderRadius, _pBorderRadius);
    painter.setClipPath(path);
    if (_pIsPreserveAspectCrop)
    {
        qreal itemAspectRatio = (qreal)rect().width() / rect().height();
        if (itemAspectRatio < _pMaximumAspectRatio)
        {
            itemAspectRatio = _pMaximumAspectRatio;
            qreal cropHeight = _pCardImage.width() / itemAspectRatio;
            painter.drawImage(QRect(0, 0, this->height() * _pMaximumAspectRatio, rect().height()), _pCardImage, QRectF(0, 0, _pCardImage.width(), cropHeight));
        }
        else
        {
            qreal cropHeight = _pCardImage.width() / itemAspectRatio;
            painter.drawImage(rect(), _pCardImage, QRectF(0, 0, _pCardImage.width(), cropHeight));
        }
    }
    else
    {
        painter.drawImage(rect(), _pCardImage);
    }
    painter.restore();
}
