#include "ElaScrollBarStyle.h"

#include "ElaColorAnimation.h"
#include "ElaScrollBar.h"
#include "ElaTheme.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStyleOptionSlider>
#include <QtMath>

ElaScrollBarStyle::ElaScrollBarStyle(QStyle* style)
{
    _pIsExpand = false;
    _pOpacity = 0;
    _pScrollBar = nullptr;
    _pSliderExtent = 2;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
    _opacityAnimation = new QPropertyAnimation(this, "pOpacity", this);
    _opacityAnimation->setDuration(83);
    connect(_opacityAnimation, &QPropertyAnimation::valueChanged, this, [this]() {
        _pScrollBar->update();
    });
    connect(_opacityAnimation, &QPropertyAnimation::finished, this, [this]() {
        _pIsExpand = _pOpacity > 0;
    });
    _extentAnimation = new QPropertyAnimation(this, "pSliderExtent", this);
    _extentAnimation->setDuration(167);
    QEasingCurve curve(QEasingCurve::BezierSpline);
    curve.addCubicBezierSegment({0, 0}, {0, 1}, {1, 1});
    _extentAnimation->setEasingCurve(curve);
    connect(_extentAnimation, &QPropertyAnimation::valueChanged, this, [this]() {
        _pScrollBar->update();
    });
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
                painter->setBrush(ElaColorAnimation::getColor(widget, ElaThemeColor(_themeMode, BasicBase), "Track"));
                painter->drawRoundedRect(scrollBarRect, 6, 6);
                //指示器绘制 center()在此处不适用 存在外围边距宽度 需手动计算
                const int sideLength = _pScrollBar->getScrollBarWidth() - 2;
                painter->setBrush(ElaColorAnimation::getColor(widget, ElaThemeColor(_themeMode, BasicIndicator), "Thumb"));
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
                    qreal centerToTop = (sideLength / 2) / qCos(30 * M_PI / 180.0);
                    qreal centerToBottom = (sideLength / 2) * qTan(30 * M_PI / 180.0);
                    // 上三角
                    qreal centerX = upIndicatorRect.width() / 2.0;
                    qreal centerUpY = upIndicatorRect.center().y() + 2;
                    qreal centerDownY = downIndicatorRect.center().y() + 2;
                    QPainterPath upPath;
                    upPath.moveTo(centerX, centerUpY - centerToTop);
                    upPath.lineTo(centerX + sideLength / 2, centerUpY + centerToBottom);
                    upPath.lineTo(centerX - sideLength / 2, centerUpY + centerToBottom);
                    upPath.closeSubpath();
                    painter->drawPath(upPath);

                    // 下三角
                    QPainterPath downPath;
                    downPath.moveTo(centerX, centerDownY + centerToBottom);
                    downPath.lineTo(centerX + sideLength / 2, centerDownY - centerToTop);
                    downPath.lineTo(centerX - sideLength / 2, centerDownY - centerToTop);
                    downPath.closeSubpath();
                    painter->drawPath(downPath);
                }
            }
            painter->setOpacity(1);
            //滑块绘制
            QRectF sliderRect = subControlRect(control, sopt, QStyle::SC_ScrollBarSlider, widget);
            painter->setBrush(ElaColorAnimation::getColor(widget, ElaThemeColor(_themeMode, BasicIndicator), "Thumb"));
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
        return _pScrollBar->getScrollBarWidth();
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
        return false;
    }
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

void ElaScrollBarStyle::startExpandAnimation(bool isExpand)
{
    _opacityAnimation->stop();
    _extentAnimation->stop();
    _pIsExpand = true;
    _opacityAnimation->setStartValue(_pOpacity);
    _opacityAnimation->setEndValue(isExpand ? 1.0 : 0.0);
    _extentAnimation->setStartValue(_pSliderExtent);
    _extentAnimation->setEndValue(isExpand ? _pScrollBar->getScrollBarWidth() - 2 * _sliderMargin : 2.0);
    _opacityAnimation->start();
    _extentAnimation->start();
}
