#include "ElaProgressBar.h"

#include <QPropertyAnimation>
#include <QStyleOption>

#include "ElaProgressBarPrivate.h"
#include "ElaProgressBarStyle.h"
ElaProgressBar::ElaProgressBar(QWidget* parent)
    : QProgressBar(parent), d_ptr(new ElaProgressBarPrivate())
{
    Q_D(ElaProgressBar);
    setObjectName("ElaProgressBar");
    setStyleSheet("#ElaProgressBar{background-color:transparent;}");
    d->q_ptr = this;
    d->_style = new ElaProgressBarStyle(style());
    setStyle(d->_style);
    d->_busyAnimation = new QPropertyAnimation(d->_style, "busyStartValue");
    connect(d->_busyAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        d->_style->setProperty("busyEndValue", value.toInt() + 75);
        update();
    });
    d->_busyAnimation->setDuration(2000);
    d->_busyAnimation->setLoopCount(-1);
}

ElaProgressBar::~ElaProgressBar()
{
    Q_D(ElaProgressBar);
    delete d->_style;
}

void ElaProgressBar::setMinimum(int minimum)
{
    Q_D(ElaProgressBar);
    if (d->_isBusyAnimation && !(maximum() == 0 && minimum == 0))
    {
        d->_isBusyAnimation = false;
        d->_busyAnimation->stop();
    }
    QProgressBar::setMinimum(minimum);
}

void ElaProgressBar::setMaximum(int maximum)
{
    Q_D(ElaProgressBar);
    if (d->_isBusyAnimation && !(minimum() == 0 && maximum == 0))
    {
        d->_isBusyAnimation = false;
        d->_busyAnimation->stop();
    }
    QProgressBar::setMaximum(maximum);
}

void ElaProgressBar::paintEvent(QPaintEvent* event)
{
    Q_D(ElaProgressBar);
    if (!d->_isBusyAnimation && minimum() == 0 && maximum() == 0)
    {
        QStyleOptionProgressBar option;
        option.initFrom(this);
        d->_isBusyAnimation = true;
        d->_busyAnimation->setStartValue(-75);
        if (orientation() == Qt::Horizontal)
        {
            d->_busyAnimation->setEndValue(this->width());
        }
        else
        {
            d->_busyAnimation->setEndValue(this->height());
        }
        d->_busyAnimation->start();
    }
    QProgressBar::paintEvent(event);
}

void ElaProgressBar::resizeEvent(QResizeEvent* event)
{
    Q_D(ElaProgressBar);
    if (orientation() == Qt::Horizontal)
    {
        d->_busyAnimation->setEndValue(this->width());
    }
    else
    {
        d->_busyAnimation->setEndValue(this->height());
    }
    QProgressBar::resizeEvent(event);
}
