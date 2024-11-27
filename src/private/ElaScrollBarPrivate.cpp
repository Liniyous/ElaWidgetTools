#include "ElaScrollBarPrivate.h"

#include <QApplication>
#include <QPropertyAnimation>
#include <QStyleOption>

#include "ElaScrollBar.h"
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
        if (max == 0)
        {
            _scrollValue = -1;
        }
        _pTargetMaximum = max;
    }
}

void ElaScrollBarPrivate::_scroll(Qt::KeyboardModifiers modifiers, int delta)
{
    Q_Q(ElaScrollBar);
    int stepsToScroll = 0;
    qreal offset = qreal(delta) / 120;
    int pageStep = 10;
    int singleStep = q->singleStep();
    if ((modifiers & Qt::ControlModifier) || (modifiers & Qt::ShiftModifier))
    {
        stepsToScroll = qBound(-pageStep, int(offset * pageStep), pageStep);
    }
    else
    {
        stepsToScroll = QApplication::wheelScrollLines() * offset * singleStep;
    }
    if (abs(_scrollValue - q->value()) > abs(stepsToScroll * _pSpeedLimit))
    {
        _scrollValue = q->value();
    }
    _scrollValue -= stepsToScroll;
    _slideSmoothAnimation->stop();
    _slideSmoothAnimation->setStartValue(q->value());
    _slideSmoothAnimation->setEndValue(_scrollValue);
    _slideSmoothAnimation->start();
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
        q->setGeometry(0, _originScrollArea->height() - _horSpace, _originScrollArea->width(), 10);
    }
    else
    {
        q->setGeometry(_originScrollArea->width() - _verSpace, 0, 10, _originScrollArea->height());
    }
}
