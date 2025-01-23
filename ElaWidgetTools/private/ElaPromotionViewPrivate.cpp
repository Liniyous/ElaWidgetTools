#include "ElaPromotionViewPrivate.h"

#include <QPropertyAnimation>

#include "ElaPromotionCard.h"
#include "ElaPromotionView.h"
ElaPromotionViewPrivate::ElaPromotionViewPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaPromotionViewPrivate::~ElaPromotionViewPrivate()
{
}

void ElaPromotionViewPrivate::onPromotionCardClicked(ElaPromotionCard* clickedCard)
{
    Q_Q(ElaPromotionView);
    int newCurrentIndex = _promotionCardList.indexOf(clickedCard);
    int oldCurrentIndex = _pCurrentIndex;
    if (newCurrentIndex == oldCurrentIndex)
    {
        return;
    }
    //卡片移动动画
    bool isRightToLeft = _promotionCardList[oldCurrentIndex]->x() < _promotionCardList[newCurrentIndex]->x();
    int originIndex = _getAdjacentIndex(Qt::RightToLeft, newCurrentIndex);
    for (int i = 0; i < _promotionCardList.count(); i++)
    {
        ElaPromotionCard* card = _promotionCardList[originIndex];
        QPropertyAnimation* geometryAnimation = new QPropertyAnimation(card, "geometry");
        QRect geometry = card->geometry();
        geometryAnimation->setEasingCurve(QEasingCurve::OutCubic);
        geometryAnimation->setDuration(650);
        geometryAnimation->setStartValue(geometry);
        if (i == 0)
        {
            //最左侧卡片
            QRect targetGeometry(-_pCardCollapseWidth + _leftPadding, 0, _pCardCollapseWidth, q->height() - _bottomMargin);
            if (_promotionCardList.count() > 2)
            {
                if (!isRightToLeft)
                {
                    geometryAnimation->setKeyValueAt(0.7, QRect(geometry.x() + _pCardCollapseWidth * 0.7 + _cardSpacing, 0, _pCardCollapseWidth, q->height() - _bottomMargin));
                    geometryAnimation->setKeyValueAt(0.71, QRect(-_pCardCollapseWidth, 0, _pCardCollapseWidth, q->height() - _bottomMargin));
                }
                geometryAnimation->setEndValue(targetGeometry);
            }
            else
            {
                if (isRightToLeft)
                {
                    geometryAnimation->setEndValue(targetGeometry);
                }
                else
                {
                    geometryAnimation->setEndValue(QRect(_leftPadding + _pCardExpandWidth + _cardSpacing * 2, 0, _pCardCollapseWidth, q->height() - _bottomMargin));
                }
            }
            _startHorizontalCardPixmapRatioAnimation(card, card->getHorizontalCardPixmapRatio(), 0.5);
        }
        else if (i == 1)
        {
            //展开的卡片
            geometryAnimation->setEndValue(QRect(_leftPadding + _cardSpacing, 0, _pCardExpandWidth, q->height() - _bottomMargin));
            _startHorizontalCardPixmapRatioAnimation(card, card->getHorizontalCardPixmapRatio(), 1);
        }
        else
        {
            QRect targetGeometry(_leftPadding + _pCardExpandWidth + _cardSpacing * i + _pCardCollapseWidth * (i - 2), 0, _pCardCollapseWidth, q->height() - _bottomMargin);
            if (isRightToLeft && targetGeometry.x() > card->x())
            {
                connect(geometryAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                    if (card->geometry().right() <= 0)
                    {
                        geometryAnimation->pause();
                        qreal timeRatio = geometryAnimation->currentTime() / 650.0;
                        geometryAnimation->setKeyValueAt(timeRatio, QRect(_promotionCardList[_getAdjacentIndex(Qt::RightToLeft, originIndex)]->geometry().right() + _pCardCollapseWidth * (1 - timeRatio), 0, _pCardCollapseWidth, q->height() - _bottomMargin));
                        geometryAnimation->setEndValue(targetGeometry);
                        geometryAnimation->resume();
                    }
                });
                geometryAnimation->setEndValue(QRect(-_pCardCollapseWidth * 2 + _leftPadding, 0, _pCardCollapseWidth, q->height() - _bottomMargin));
            }
            else
            {
                geometryAnimation->setEndValue(targetGeometry);
            }
            card->setHorizontalCardPixmapRatio(0.5);
        }
        originIndex = _getAdjacentIndex(Qt::LeftToRight, originIndex);
        geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    _pCurrentIndex = newCurrentIndex;
    q->update();
    Q_EMIT q->pCurrentIndexChanged();
}

void ElaPromotionViewPrivate::_startCardGeometryAnimation(ElaPromotionCard* card, QRect start, QRect end)
{
    QPropertyAnimation* geometryAnimation = new QPropertyAnimation(card, "geometry");
    geometryAnimation->setEasingCurve(QEasingCurve::OutCubic);
    geometryAnimation->setDuration(650);
    geometryAnimation->setStartValue(start);
    geometryAnimation->setEndValue(end);
    geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaPromotionViewPrivate::_startHorizontalCardPixmapRatioAnimation(ElaPromotionCard* card, qreal start, qreal end)
{
    QPropertyAnimation* ratioAnimation = new QPropertyAnimation(card, "pHorizontalCardPixmapRatio");
    ratioAnimation->setEasingCurve(QEasingCurve::OutCubic);
    ratioAnimation->setDuration(650);
    ratioAnimation->setStartValue(start);
    ratioAnimation->setEndValue(end);
    ratioAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaPromotionViewPrivate::_updatePromotionCardGeometry()
{
    Q_Q(ElaPromotionView);
    if (_promotionCardList.count() < 3)
    {
        if (_promotionCardList.count() == 1)
        {
            _promotionCardList[0]->setGeometry(_leftPadding + _cardSpacing, 0, _pCardExpandWidth, q->height() - _bottomMargin);
        }
        else
        {
            _promotionCardList[0]->setGeometry(_leftPadding + _cardSpacing, 0, _pCardExpandWidth, q->height() - _bottomMargin);
            QRect lastGeometry = _promotionCardList[0]->geometry();
            _promotionCardList[1]->setGeometry(QRect(lastGeometry.right() + _cardSpacing, lastGeometry.y(), _pCardCollapseWidth, lastGeometry.height()));
            _promotionCardList[1]->setHorizontalCardPixmapRatio(0.5);
        }
    }
    else
    {
        for (int i = 0; i < _promotionCardList.count(); i++)
        {
            if (i == 0)
            {
                //展开的卡片
                _promotionCardList[i]->setGeometry(_leftPadding + _cardSpacing, 0, _pCardExpandWidth, q->height() - _bottomMargin);
                _promotionCardList[i]->setHorizontalCardPixmapRatio(1);
            }
            else if (i == _promotionCardList.count() - 1)
            {
                //最左侧卡片
                _promotionCardList[i]->setGeometry(-_pCardCollapseWidth + _leftPadding, 0, _pCardCollapseWidth, q->height() - _bottomMargin);
                _promotionCardList[i]->setHorizontalCardPixmapRatio(0.5);
            }
            else
            {
                QRect lastGeometry = _promotionCardList[i - 1]->geometry();
                _promotionCardList[i]->setGeometry(QRect(lastGeometry.right() + _cardSpacing, lastGeometry.y(), _pCardCollapseWidth, lastGeometry.height()));
                _promotionCardList[i]->setHorizontalCardPixmapRatio(0.5);
            }
        }
    }
}

int ElaPromotionViewPrivate::_getAdjacentIndex(Qt::LayoutDirection direction, int index)
{
    if (direction == Qt::LeftToRight)
    {
        if (index == _promotionCardList.count() - 1)
        {
            return 0;
        }
        return index + 1;
    }
    else
    {
        if (index > 0)
        {
            return index - 1;
        }
        return _promotionCardList.count() - 1;
    }
}

int ElaPromotionViewPrivate::_getRightLimitX()
{
    int count = _promotionCardList.count();
    return _leftPadding + _pCardExpandWidth + _cardSpacing * (count - 1) + _pCardCollapseWidth * (count - 3);
}
