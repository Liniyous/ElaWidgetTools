#include "ElaProgressRing.h"

#include "ElaProgressRingPrivate.h"
#include "ElaTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QtMath>
Q_PROPERTY_CREATE_Q_CPP(ElaProgressRing, bool, IsTransparent)
Q_PROPERTY_CREATE_Q_CPP(ElaProgressRing, bool, IsDisplayValue)
Q_PROPERTY_CREATE_Q_CPP(ElaProgressRing, ElaProgressRingType::ValueDisplayMode, ValueDisplayMode)
Q_PROPERTY_CREATE_Q_CPP(ElaProgressRing, int, ValuePixelSize)
ElaProgressRing::ElaProgressRing(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaProgressRingPrivate())
{
    Q_D(ElaProgressRing);
    d->q_ptr = this;
    d->_pIsDisplayValue = true;
    d->_pValuePixelSize = 17;
    d->_pValueDisplayMode = ElaProgressRingType::ValueDisplayMode::Actual;
    d->_pMinimum = 0;
    d->_pMaximum = 100;
    d->_pValue = 0;
    d->_pBusyIndex = 0xe100;
    d->_pBusyingDurationTime = 1000;
    d->_pBusyingWidth = 6;
    d->_pBusyStartDeg = 0;
    d->_pBusyContentDeg = 0;
    d->_pIsBusying = false;
    d->_pIsTransparent = false;
    setFixedSize(70, 70);
    setObjectName("ElaProgressRing");
    setStyleSheet("#ElaProgressRing{background-color:transparent;}");

    d->_busyStartDegAnimation = new QPropertyAnimation(d, "pBusyStartDeg");
    connect(d->_busyStartDegAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
        update();
    });
    d->_busyStartDegAnimation->setEasingCurve(QEasingCurve::Linear);
    d->_busyStartDegAnimation->setDuration(1000);
    d->_busyStartDegAnimation->setStartValue(0);
    d->_busyStartDegAnimation->setEndValue(360);
    d->_busyStartDegAnimation->setLoopCount(-1);

    d->_busyContentDegAnimation = new QPropertyAnimation(d, "pBusyContentDeg");
    d->_busyContentDegAnimation->setEasingCurve(QEasingCurve::Linear);
    d->_busyContentDegAnimation->setDuration(1800);
    d->_busyContentDegAnimation->setStartValue(10);
    d->_busyContentDegAnimation->setKeyValueAt(0.5, 140);
    d->_busyContentDegAnimation->setEndValue(10);
    d->_busyContentDegAnimation->setLoopCount(-1);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
}

ElaProgressRing::~ElaProgressRing()
{
}

void ElaProgressRing::setIsBusying(bool isBusying)
{
    Q_D(ElaProgressRing);
    d->_pIsBusying = isBusying;
    if (isBusying)
    {
        d->_busyStartDegAnimation->start();
        d->_busyContentDegAnimation->start();
    }
    else
    {
        d->_busyStartDegAnimation->stop();
        d->_busyContentDegAnimation->stop();
    }
    update();
    Q_EMIT pIsBusyingChanged();
}

bool ElaProgressRing::getIsBusying() const
{
    Q_D(const ElaProgressRing);
    return d->_pIsBusying;
}

void ElaProgressRing::setBusyingWidth(int width)
{
    Q_D(ElaProgressRing);
    d->_pBusyingWidth = width;
    update();
    Q_EMIT pBusyingWidthChanged();
}

int ElaProgressRing::getBusyingWidth() const
{
    Q_D(const ElaProgressRing);
    return d->_pBusyingWidth;
}

void ElaProgressRing::setBusyingDurationTime(int busyingDurationTime)
{
    Q_D(ElaProgressRing);
    d->_pBusyingDurationTime = busyingDurationTime;
    d->_busyStartDegAnimation->setDuration(d->_pBusyingDurationTime);
    Q_EMIT pBusyingDurationTimeChanged();
}

int ElaProgressRing::getBusyingDurationTime() const
{
    Q_D(const ElaProgressRing);
    return d->_pBusyingDurationTime;
}

void ElaProgressRing::setMinimum(int minimum)
{
    Q_D(ElaProgressRing);
    d->_pMinimum = minimum;
    update();
    Q_EMIT pMinimumChanged();
    Q_EMIT rangeChanged(d->_pMinimum, d->_pMaximum);
}

int ElaProgressRing::getMinimum() const
{
    Q_D(const ElaProgressRing);
    return d->_pMinimum;
}

void ElaProgressRing::setMaximum(int maximum)
{
    Q_D(ElaProgressRing);
    d->_pMaximum = maximum;
    update();
    Q_EMIT pMaximumChanged();
    Q_EMIT rangeChanged(d->_pMinimum, d->_pMaximum);
}

int ElaProgressRing::getMaximum() const
{
    Q_D(const ElaProgressRing);
    return d->_pMaximum;
}

void ElaProgressRing::setValue(int value)
{
    Q_D(ElaProgressRing);
    if (value < d->_pMinimum || value > d->_pMaximum)
    {
        return;
    }
    d->_pValue = value;
    update();
    Q_EMIT pValueChanged();
}

int ElaProgressRing::getValue() const
{
    Q_D(const ElaProgressRing);
    return d->_pValue;
}

void ElaProgressRing::setRange(int min, int max)
{
    Q_D(ElaProgressRing);
    if (min < 0 || max < 0 || min > max)
    {
        return;
    }
    d->_pMinimum = min;
    d->_pMaximum = max;
    update();
    Q_EMIT pMinimumChanged();
    Q_EMIT pMaximumChanged();
    Q_EMIT rangeChanged(min, max);
}

void ElaProgressRing::paintEvent(QPaintEvent* event)
{
    Q_D(ElaProgressRing);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    // 文字绘制
    if (d->_pIsDisplayValue && !d->_pIsBusying)
    {
        painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
        QString valueText;
        if (d->_pValueDisplayMode == ElaProgressRingType::ValueDisplayMode::Actual)
        {
            valueText = QString::number(d->_pValue);
        }
        else
        {
            valueText = QString::number(d->_pValue / (qreal)(d->_pMaximum - d->_pMinimum) * 100) + "%";
        }
        painter.drawText(rect(), Qt::AlignCenter | Qt::TextSingleLine, valueText);
    }
    // Ring绘制
    QRect ringRect = rect().adjusted(1, 1, -1, -1);
    // 遮罩
    QPainterPath basePath;
    basePath.addEllipse(ringRect);
    QPainterPath maskPath;
    maskPath.addEllipse(ringRect.adjusted(d->_pBusyingWidth, d->_pBusyingWidth, -d->_pBusyingWidth, -d->_pBusyingWidth));
    painter.setClipPath(basePath.subtracted(maskPath));
    painter.setPen(Qt::NoPen);
    // 底色
    if (!d->_pIsTransparent)
    {
        painter.setBrush(ElaThemeColor(d->_themeMode, BasicBaseDeep));
        painter.drawEllipse(ringRect);
    }
    // 环
    painter.setBrush(ElaThemeColor(d->_themeMode, PrimaryNormal));
    if (d->_pIsBusying)
    {
        painter.drawPie(ringRect, -d->_pBusyStartDeg * 16, d->_pBusyContentDeg * 16);
    }
    else
    {
        qreal valueDeg = d->_pValue / (qreal)(d->_pMaximum - d->_pMinimum) * 360;
        painter.drawPie(ringRect, 90 * 16, -valueDeg * 16);
    }
    painter.restore();
}
