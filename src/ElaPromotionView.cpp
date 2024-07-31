#include "ElaPromotionView.h"

#include <QPainter>
#include <QTimer>
#include <QWheelEvent>

#include "ElaPromotionCard.h"
#include "ElaPromotionViewPrivate.h"
#include "ElaTheme.h"
ElaPromotionView::ElaPromotionView(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaPromotionViewPrivate())
{
    Q_D(ElaPromotionView);
    d->q_ptr = this;
    d->_pCurrentIndex = 0;
    d->_pCardExpandWidth = 600;
    d->_pCardCollapseWidth = 300;
    d->_pIsAutoScroll = false;
    d->_pAutoScrollInterval = 5000;
    setFixedHeight(300);
    setObjectName("ElaPromotionView");
    setStyleSheet("#ElaPromotionView{background-color:transparent;}");

    d->_autoScrollTimer = new QTimer(this);
    connect(d->_autoScrollTimer, &QTimer::timeout, this, [=]() {
        if (isVisible() && d->_promotionCardList.count() > 2)
        {
            d->onPromotionCardClicked(d->_promotionCardList[d->_getAdjacentIndex(Qt::LeftToRight, d->_pCurrentIndex)]);
        }
    });

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaPromotionView::~ElaPromotionView()
{
}

void ElaPromotionView::setCardExpandWidth(int width)
{
    Q_D(ElaPromotionView);
    if (width <= 0)
    {
        return;
    }
    d->_pCardExpandWidth = width;
    d->_updatePromotionCardGeometry();
}

int ElaPromotionView::getCardExpandWidth() const
{
    Q_D(const ElaPromotionView);
    return d->_pCardExpandWidth;
}

void ElaPromotionView::setCardCollapseWidth(int width)
{
    Q_D(ElaPromotionView);
    if (width <= 0)
    {
        return;
    }
    d->_pCardCollapseWidth = width;
    d->_updatePromotionCardGeometry();
}

int ElaPromotionView::getCardCollapseWidth() const
{
    Q_D(const ElaPromotionView);
    return d->_pCardCollapseWidth;
}

void ElaPromotionView::setCurrentIndex(int index)
{
    Q_D(ElaPromotionView);
    if (index < 0 || index >= d->_promotionCardList.count())
    {
        return;
    }
    d->onPromotionCardClicked(d->_promotionCardList[index]);
}

int ElaPromotionView::getCurrentIndex() const
{
    Q_D(const ElaPromotionView);
    return d->_pCurrentIndex;
}

void ElaPromotionView::setIsAutoScroll(bool isAutoScroll)
{
    Q_D(ElaPromotionView);
    if (isAutoScroll)
    {
        d->_autoScrollTimer->start(d->_pAutoScrollInterval);
    }
    else
    {
        d->_autoScrollTimer->stop();
    }
    d->_pIsAutoScroll = isAutoScroll;
    Q_EMIT pIsAutoScrollChanged();
}

bool ElaPromotionView::getIsAutoScroll() const
{
    Q_D(const ElaPromotionView);
    return d->_pIsAutoScroll;
}

void ElaPromotionView::setAutoScrollInterval(int autoScrollInterval)
{
    Q_D(ElaPromotionView);
    if (autoScrollInterval < 400)
    {
        return;
    }
    d->_pAutoScrollInterval = autoScrollInterval;
    Q_EMIT pAutoScrollIntervalChanged();
}

int ElaPromotionView::getAutoScrollInterval() const
{
    Q_D(const ElaPromotionView);
    return d->_pAutoScrollInterval;
}

void ElaPromotionView::appendPromotionCard(ElaPromotionCard* card)
{
    Q_D(ElaPromotionView);
    if (!card || d->_promotionCardList.contains(card))
    {
        return;
    }
    card->setMinimumSize(0, 0);
    card->setMaximumSize(10000, 10000);
    card->setParent(this);
    d->_promotionCardList.append(card);
    connect(card, &ElaPromotionCard::promotionCardClicked, this, [=]() {
        d->onPromotionCardClicked(card);
    });
    d->_updatePromotionCardGeometry();
}

void ElaPromotionView::wheelEvent(QWheelEvent* event)
{
    Q_D(ElaPromotionView);
    if (d->_isAllowSwitch)
    {
        if (event->angleDelta().y() > 0)
        {
            //右滑
            d->_isAllowSwitch = false;
            QTimer::singleShot(400, this, [=] {
                d->_isAllowSwitch = true;
            });
            d->onPromotionCardClicked(d->_promotionCardList[d->_getAdjacentIndex(Qt::RightToLeft, d->_pCurrentIndex)]);
        }
        else
        {
            //左滑
            d->_isAllowSwitch = false;
            QTimer::singleShot(400, this, [=] {
                d->_isAllowSwitch = true;
            });
            d->onPromotionCardClicked(d->_promotionCardList[d->_getAdjacentIndex(Qt::LeftToRight, d->_pCurrentIndex)]);
        }
    }
}

void ElaPromotionView::paintEvent(QPaintEvent* event)
{
    Q_D(ElaPromotionView);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaThemeColor(d->_themeMode, PromotionViewIndicator));
    //页标指示器绘制
    int promotionCardCount = d->_promotionCardList.count();
    bool isCountOdd = promotionCardCount % 2;
    QPoint startPoint = isCountOdd ? QPoint(width() / 2 - promotionCardCount / 2 * d->_indicatorSpacing, height() - d->_bottomMargin / 2) : QPoint(width() / 2 - promotionCardCount / 2 * d->_indicatorSpacing - d->_indicatorSpacing / 2, height() - d->_bottomMargin / 2);
    for (int i = 0; i < promotionCardCount; i++)
    {
        if (i == d->_pCurrentIndex)
        {
            painter.drawEllipse(QPointF(startPoint.x() + i * d->_indicatorSpacing, startPoint.y()), 3.5, 3.5);
        }
        else
        {
            painter.drawEllipse(QPointF(startPoint.x() + i * d->_indicatorSpacing, startPoint.y()), 2.5, 2.5);
        }
    }
    painter.restore();
}
