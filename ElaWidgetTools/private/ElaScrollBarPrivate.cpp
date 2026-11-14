#include "ElaScrollBarPrivate.h"

#include "ElaScrollAnimation.h"
#include "ElaScrollBar.h"

#include <QPropertyAnimation>
#include <QStyleOption>

ElaScrollBarPrivate::ElaScrollBarPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaScrollBarPrivate::~ElaScrollBarPrivate()
{
}

void ElaScrollBarPrivate::onRangeChanged(int min, int max)
{
    Q_Q(ElaScrollBar);
    _scrollAnimation->stop();
    if (q->isVisible() && _pIsAnimation && max != 0)
    {
        QPropertyAnimation* rangeSmoothAnimation = new QPropertyAnimation(this, "pTargetMaximum");
        connect(rangeSmoothAnimation, &QPropertyAnimation::finished, this, [=]() {
            Q_EMIT q->rangeAnimationFinished();
        });
        connect(rangeSmoothAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            q->blockSignals(true);
            q->setMaximum(value.toUInt());
            q->blockSignals(false);
            q->update();
        });
        rangeSmoothAnimation->setEasingCurve(QEasingCurve::OutSine);
        rangeSmoothAnimation->setDuration(250);
        rangeSmoothAnimation->setStartValue(_pTargetMaximum);
        rangeSmoothAnimation->setEndValue(max);
        rangeSmoothAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        _pTargetMaximum = max;
    }
}

bool ElaScrollBarPrivate::_scroll(qreal movement)
{
    Q_Q(ElaScrollBar);
    _scrollScale = _getScrollScale();
    return _scrollAnimation->scrollBy({movement * _scrollScale, 0}, {q->value() * _scrollScale, 0}, {q->minimum() * _scrollScale, 0}, {q->maximum() * _scrollScale, 0}, _pSpeedLimit);
}

qreal ElaScrollBarPrivate::_getScrollScale() const
{
    Q_Q(const ElaScrollBar);
    for (auto* widget = q->parentWidget(); widget; widget = widget->parentWidget())
    {
        if (auto* area = qobject_cast<QAbstractScrollArea*>(widget))
        {
            const int extent = q->orientation() == Qt::Vertical ? area->viewport()->height() : area->viewport()->width();
            return qMax(1.0, extent / static_cast<qreal>(qMax(1, q->pageStep())));
        }
    }
    return 1;
}

int ElaScrollBarPrivate::_pixelPosToRangeValue(int pos) const
{
    Q_Q(const ElaScrollBar);
    QStyleOptionSlider opt;
    q->initStyleOption(&opt);
    QRect gr = q->style()->subControlRect(QStyle::CC_ScrollBar, &opt,
                                          QStyle::SC_ScrollBarGroove, q);
    QRect sr = q->style()->subControlRect(QStyle::CC_ScrollBar, &opt,
                                          QStyle::SC_ScrollBarSlider, q);
    int sliderMin, sliderMax, sliderLength;
    if (q->orientation() == Qt::Horizontal)
    {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
        if (q->layoutDirection() == Qt::RightToLeft)
        {
            opt.upsideDown = !opt.upsideDown;
        }
    }
    else
    {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    return QStyle::sliderValueFromPosition(q->minimum(), q->maximum(), pos - sliderMin,
                                           sliderMax - sliderMin, opt.upsideDown);
}

void ElaScrollBarPrivate::_initAllConfig()
{
    Q_Q(ElaScrollBar);
    _handleScrollBarRangeChanged(_originScrollBar->minimum(), _originScrollBar->maximum());
    q->setSingleStep(_originScrollBar->singleStep());
    q->setPageStep(_originScrollBar->pageStep());
}

void ElaScrollBarPrivate::_handleScrollBarValueChanged(QScrollBar* scrollBar, int value)
{
    scrollBar->setValue(value);
}

void ElaScrollBarPrivate::_handleScrollBarRangeChanged(int min, int max)
{
    Q_Q(ElaScrollBar);
    q->setRange(min, max);
    if (max <= 0)
    {
        q->setVisible(false);
    }
    else
    {
        q->setVisible(true);
    }
}

void ElaScrollBarPrivate::_handleScrollBarGeometry()
{
    Q_Q(ElaScrollBar);
    q->raise();
    q->setSingleStep(_originScrollBar->singleStep());
    q->setPageStep(_originScrollBar->pageStep());
    if (q->orientation() == Qt::Horizontal)
    {
        const int extent = q->sizeHint().height();
        q->setGeometry(0, _originScrollArea->height() - extent, _originScrollArea->width(), extent);
    }
    else
    {
        const int extent = q->sizeHint().width();
        q->setGeometry(_originScrollArea->width() - extent, 0, extent, _originScrollArea->height());
    }
}
