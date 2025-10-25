#include "ElaImageCard.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>

#include "ElaImageCardPrivate.h"
#include "ElaTheme.h"

Q_PROPERTY_CREATE_Q_CPP(ElaImageCard, QImage, CardImage);
Q_PROPERTY_CREATE_Q_CPP(ElaImageCard, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaImageCard, bool, IsPreserveAspectCrop)
ElaImageCard::ElaImageCard(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaImageCardPrivate())
{
    Q_D(ElaImageCard);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
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
        qreal windowAspectRatio = (qreal)rect().width() / rect().height();
        qreal pixAspectRatio = (qreal)d->_pCardImage.width() / d->_pCardImage.height();
        int targetPixWidth, targetPixHeight;
        if (windowAspectRatio < pixAspectRatio)
        {
            targetPixWidth = qRound(d->_pCardImage.width() * windowAspectRatio / pixAspectRatio);
            targetPixHeight = d->_pCardImage.height();
        }
        else
        {
            targetPixWidth = d->_pCardImage.width();
            targetPixHeight = qRound(d->_pCardImage.height() * pixAspectRatio / windowAspectRatio);
        }
        painter.drawImage(rect(), d->_pCardImage, QRect((d->_pCardImage.width() - targetPixWidth) / 2, (d->_pCardImage.height() - targetPixHeight) / 2, targetPixWidth, targetPixHeight));
    }
    else
    {
        painter.drawImage(rect(), d->_pCardImage);
    }
    painter.restore();
}
