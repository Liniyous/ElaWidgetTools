#include "ElaScrollBarStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStyleOptionSlider>
#include <QtMath>

#include "ElaScrollBar.h"
#include "ElaTheme.h"
ElaScrollBarStyle::ElaScrollBarStyle(QStyle* style)
{
    _pIsExpand = false;
    _pOpacity = 0;
    _pScrollBar = nullptr;
    _pSliderExtent = 3;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaScrollBarStyle::~ElaScrollBarStyle()
{
}

void ElaScrollBarStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    //QStyle::SC_ScrollBarGroove QStyle::SC_ScrollBarAddLine   QStyle::SC_ScrollBarSubLine上指示器
    switch (control)
    {
    case QStyle::CC_ScrollBar:
    {
        if (const QStyleOptionSlider* sopt = qstyleoption_cast<const QStyleOptionSlider*>(option))
        {
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            QRect scrollBarRect = sopt->rect;
            if (_pIsExpand)
            {
                // 背景绘制
                painter->setOpacity(_pOpacity);
                painter->setPen(Qt::NoPen);
                painter->setBrush(ElaThemeColor(_themeMode, ScrollBarBase));
                painter->drawRoundedRect(scrollBarRect, 6, 6);
                //指示器绘制 center()在此处不适用 存在外围边距宽度 需手动计算
                int sideLength = 8;
                painter->setBrush(ElaThemeColor(_themeMode, ScrollBarHandle));
                if (sopt->orientation == Qt::Horizontal)
                {
                    QRect leftIndicatorRect = subControlRect(control, sopt, QStyle::SC_ScrollBarSubLine, widget);
                    QRect rightIndicatorRect = subControlRect(control, sopt, QStyle::SC_ScrollBarAddLine, widget);
                    // 左三角
                    qreal centerLeftX = leftIndicatorRect.x() + leftIndicatorRect.width() / 2;
                    qreal centerRightX = rightIndicatorRect.x() + rightIndicatorRect.width() / 2;
                    qreal centerY = leftIndicatorRect.height() / 2;
                    QPainterPath leftPath;
                    leftPath.moveTo(centerLeftX - qCos(30 * M_PI / 180.0) * sideLength / 2, centerY);
                    leftPath.lineTo(centerLeftX + qCos(30 * M_PI / 180.0) * sideLength / 2, centerY + sideLength / 2);
                    leftPath.lineTo(centerLeftX + qCos(30 * M_PI / 180.0) * sideLength / 2, centerY - sideLength / 2);
                    leftPath.closeSubpath();
                    painter->drawPath(leftPath);

                    // 右三角
                    QPainterPath rightPath;
                    rightPath.moveTo(centerRightX + qCos(30 * M_PI / 180.0) * sideLength / 2, centerY);
                    rightPath.lineTo(centerRightX - qCos(30 * M_PI / 180.0) * sideLength / 2, centerY + sideLength / 2);
                    rightPath.lineTo(centerRightX - qCos(30 * M_PI / 180.0) * sideLength / 2, centerY - sideLength / 2);
                    rightPath.closeSubpath();
                    painter->drawPath(rightPath);
                }
                else
                {
                    QRect upIndicatorRect = subControlRect(control, sopt, QStyle::SC_ScrollBarSubLine, widget);
                    QRect downIndicatorRect = subControlRect(control, sopt, QStyle::SC_ScrollBarAddLine, widget);
                    // 上三角
                    qreal centerX = upIndicatorRect.width() / 2;
                    qreal centerUpY = upIndicatorRect.bottom() - upIndicatorRect.height() / 2;
                    qreal centerDownY = downIndicatorRect.bottom() - downIndicatorRect.height() / 2;
                    QPainterPath upPath;
                    upPath.moveTo(centerX - qCos(30 * M_PI / 180.0) * sideLength / 2, centerUpY + sideLength / 2);
                    upPath.lineTo(centerX + qCos(30 * M_PI / 180.0) * sideLength / 2, centerUpY + sideLength / 2);
                    upPath.lineTo(centerX, centerUpY - sideLength / 2);
                    upPath.closeSubpath();
                    painter->drawPath(upPath);

                    // 下三角
                    QPainterPath downPath;
                    downPath.moveTo(centerX, centerDownY + sideLength / 2);
                    downPath.lineTo(centerX + qCos(30 * M_PI / 180.0) * sideLength / 2, centerDownY - sideLength / 2);
                    downPath.lineTo(centerX - qCos(30 * M_PI / 180.0) * sideLength / 2, centerDownY - sideLength / 2);
                    downPath.closeSubpath();
                    painter->drawPath(downPath);
                }
            }
            painter->setOpacity(1);
            //滑块绘制
            QRectF sliderRect = subControlRect(control, sopt, QStyle::SC_ScrollBarSlider, widget);
            painter->setBrush(ElaThemeColor(_themeMode, ScrollBarHandle));
            if (sopt->orientation == Qt::Horizontal)
            {
                sliderRect.setRect(sliderRect.x(), sliderRect.bottom() - _sliderMargin - _pSliderExtent, sliderRect.width(), _pSliderExtent);
            }
            else
            {
                sliderRect.setRect(sliderRect.right() - _sliderMargin - _pSliderExtent, sliderRect.y(), _pSliderExtent, sliderRect.height());
            }
            painter->drawRoundedRect(sliderRect, _pSliderExtent / 2.0, _pSliderExtent / 2.0);
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

int ElaScrollBarStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    // qDebug() << metric << QProxyStyle::pixelMetric(metric, option, widget);
    switch (metric)
    {
    case QStyle::PM_ScrollBarExtent:
    {
        return 12;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

int ElaScrollBarStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const
{
    if (hint == QStyle::SH_ScrollBar_LeftClickAbsolutePosition)
    {
        return true;
    }
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

void ElaScrollBarStyle::startExpandAnimation(bool isExpand)
{
    if (isExpand)
    {
        _pIsExpand = true;
        QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "pOpacity");
        connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            _pScrollBar->update();
        });
        opacityAnimation->setDuration(250);
        opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        opacityAnimation->setStartValue(_pOpacity);
        opacityAnimation->setEndValue(1);
        opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* extentAnimation = new QPropertyAnimation(this, "pSliderExtent");
        extentAnimation->setDuration(250);
        extentAnimation->setEasingCurve(QEasingCurve::InOutSine);
        extentAnimation->setStartValue(_pSliderExtent);
        extentAnimation->setEndValue(12 - 2 * _sliderMargin);
        extentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "pOpacity");
        connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() {
            _pIsExpand = false;
        });
        connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            _pScrollBar->update();
        });
        opacityAnimation->setDuration(250);
        opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        opacityAnimation->setStartValue(_pOpacity);
        opacityAnimation->setEndValue(0);
        opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* extentAnimation = new QPropertyAnimation(this, "pSliderExtent");
        extentAnimation->setDuration(250);
        extentAnimation->setEasingCurve(QEasingCurve::InOutSine);
        extentAnimation->setStartValue(_pSliderExtent);
        extentAnimation->setEndValue(3);
        extentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
