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
    d->_pAutoClockFormat = "yyyy-MM-dd hh:mm:ss";
    setDigitCount(d->_pAutoClockFormat.length());
    setSegmentStyle(QLCDNumber::Flat);
    setObjectName("ElaLCDNumber");
    setStyleSheet("#ElaLCDNumber{background-color:transparent;}");
    d->_lcdNumberStyle = new ElaLCDNumberStyle();
    setStyle(d->_lcdNumberStyle);
    d->_clockTimer = new QTimer(this);
    connect(d->_clockTimer, &QTimer::timeout, this, [=]() {
        display(QDateTime::currentDateTime().toString(d->_pAutoClockFormat));
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
    Q_D(ElaLCDNumber);
    delete d->_lcdNumberStyle;
}

void ElaLCDNumber::setIsUseAutoClock(bool isUseAutoClock)
{
    Q_D(ElaLCDNumber);
    d->_pIsUseAutoClock = isUseAutoClock;
    if (d->_pIsUseAutoClock)
    {
        setDigitCount(d->_pAutoClockFormat.length());
        display(QDateTime::currentDateTime().toString(d->_pAutoClockFormat));
        d->_clockTimer->start(200);
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

void ElaLCDNumber::setAutoClockFormat(QString autoClockFormat)
{
    Q_D(ElaLCDNumber);
    d->_pAutoClockFormat = autoClockFormat;
    setDigitCount(d->_pAutoClockFormat.length());
    Q_EMIT pAutoClockFormatChanged();
}

QString ElaLCDNumber::getAutoClockFormat() const
{
    Q_D(const ElaLCDNumber);
    return d->_pAutoClockFormat;
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
