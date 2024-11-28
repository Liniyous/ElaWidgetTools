#include "ElaImageCard.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>

#include "ElaImageCardPrivate.h"
#include "ElaTheme.h"

Q_PROPERTY_CREATE_Q_CPP(ElaImageCard, QImage, CardImage);
Q_PROPERTY_CREATE_Q_CPP(ElaImageCard, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaImageCard, bool, IsPreserveAspectCrop)
Q_PROPERTY_CREATE_Q_CPP(ElaImageCard, qreal, MaximumAspectRatio)
ElaImageCard::ElaImageCard(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaImageCardPrivate())
{
    Q_D(ElaImageCard);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    d->_pMaximumAspectRatio = 2.2;
    d->_pIsPreserveAspectCrop = true;
    setMinimumSize(350, 260);
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaImageCard::~ElaImageCard()
{
}

void ElaImageCard::paintEvent(QPaintEvent* event)
{
    Q_D(ElaImageCard);
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
    painter.setClipPath(path);
    // 图片绘制
    if (d->_pIsPreserveAspectCrop)
    {
        qreal itemAspectRatio = (qreal)rect().width() / rect().height();
        if (itemAspectRatio < d->_pMaximumAspectRatio)
        {
            itemAspectRatio = d->_pMaximumAspectRatio;
            qreal cropHeight = d->_pCardImage.width() / itemAspectRatio;
            painter.drawImage(QRect(0, 0, this->height() * d->_pMaximumAspectRatio, rect().height()), d->_pCardImage, QRectF(0, 0, d->_pCardImage.width(), cropHeight));
        }
        else
        {
            qreal cropHeight = d->_pCardImage.width() / itemAspectRatio;
            painter.drawImage(rect(), d->_pCardImage, QRectF(0, 0, d->_pCardImage.width(), cropHeight));
        }
    }
    else
    {
        painter.drawImage(rect(), d->_pCardImage);
    }
    painter.restore();
}
