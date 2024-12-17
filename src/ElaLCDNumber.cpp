#include "ElaLCDNumber.h"
#include "ElaLCDNumberPrivate.h"
#include "ElaLCDNumberStyle.h"
#include "ElaTheme.h"
#include <QDateTime>
#include <QDebug>
#include <QTimer>
ElaLCDNumber::ElaLCDNumber(QWidget* parent)
    : QLCDNumber(parent), d_ptr(new ElaLCDNumberPrivate)
{
    Q_D(ElaLCDNumber);
    d->q_ptr = this;
    d->_pIsUseAutoClock = false;
    setSegmentStyle(QLCDNumber::Flat);
    setObjectName("ElaLCDNumber");
    setStyleSheet("#ElaLCDNumber{background-color:transparent;}");
    d->_lcdNumberStyle = new ElaLCDNumberStyle();
    setStyle(d->_lcdNumberStyle);
    d->_clockTimer = new QTimer(this);
    connect(d->_clockTimer, &QTimer::timeout, this, [=]() {
        display(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    });

    d->onThemeModeChanged(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaLCDNumberPrivate::onThemeModeChanged);
}

ElaLCDNumber::ElaLCDNumber(uint numDigits, QWidget* parent)
    : ElaLCDNumber(parent)
{
    setDigitCount(numDigits);
}

ElaLCDNumber::~ElaLCDNumber()
{
}

void ElaLCDNumber::setIsUseAutoClock(bool isUseAutoClock)
{
    Q_D(ElaLCDNumber);
    d->_pIsUseAutoClock = isUseAutoClock;
    if (d->_pIsUseAutoClock)
    {
        display(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        d->_clockTimer->start(200);
        setDigitCount(19);
    }
    else
    {
        d->_clockTimer->stop();
        display("");
    }
    Q_EMIT pIsUseAutoClockChanged();
}

bool ElaLCDNumber::getIsUseAutoClock() const
{
    Q_D(const ElaLCDNumber);
    return d->_pIsUseAutoClock;
}

void ElaLCDNumber::setIsTransparent(bool isTransparent)
{
    Q_D(ElaLCDNumber);
    d->_lcdNumberStyle->setIsTransparent(isTransparent);
    update();
    Q_EMIT pIsTransparentChanged();
}

bool ElaLCDNumber::getIsTransparent() const
{
    Q_D(const ElaLCDNumber);
    return d->_lcdNumberStyle->getIsTransparent();
}
