#include "ElaRadioButtonStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaTheme.h"

ElaRadioButtonStyle::ElaRadioButtonStyle(QStyle *style) {
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaRadioButtonStyle::~ElaRadioButtonStyle() {
}

void ElaRadioButtonStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                        const QWidget *widget) const {
    switch (element) {
        case PE_IndicatorRadioButton: {
            const QStyleOptionButton *bopt = qstyleoption_cast<const QStyleOptionButton *>(option);
            if (!bopt) {
                break;
            }
            QRect buttonRect = bopt->rect;
            buttonRect.adjust(1, 1, -1, -1);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            bool isEnabled = bopt->state & QStyle::State_Enabled;
            if (bopt->state & QStyle::State_Off) {
                painter->setPen(QPen(isEnabled
                                         ? ElaThemeColor(_themeMode, BasicBorder)
                                         : ElaThemeColor(_themeMode, BasicDisable), 1.5));
                if (!isEnabled) {
                    painter->setBrush(ElaThemeColor(_themeMode, BasicDisable));
                } else if (bopt->state & QStyle::State_MouseOver) {
                    painter->setBrush(ElaThemeColor(_themeMode, BasicHover));
                } else {
                    painter->setBrush(ElaThemeColor(_themeMode, BasicBase));
                }
                painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), 8.5, 8.5);
            } else {
                painter->setPen(Qt::NoPen);
                // 外圆形
                painter->setBrush(isEnabled ?
                ElaThemeColor(_themeMode, PrimaryNormal) :
                ElaThemeColor(_themeMode, PrimaryNormal));
                painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1),
                                     buttonRect.width() / 2, buttonRect.width() / 2);
                // 内圆形
                painter->setBrush(isEnabled ?
                ElaThemeColor(_themeMode, BasicTextInvert) :
                ElaThemeColor(_themeMode, BasicTextDisable));
                if (isEnabled) {
                    if (bopt->state & QStyle::State_Sunken) {
                        if (bopt->state & QStyle::State_MouseOver) {
                            painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1),
                                                 buttonRect.width() / 4.5, buttonRect.width() / 4.5);
                        }
                    } else {
                        if (bopt->state & QStyle::State_MouseOver) {
                            painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1),
                                                 buttonRect.width() / 3.5, buttonRect.width() / 3.5);
                        } else {
                            painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1),
                                                 buttonRect.width() / 4, buttonRect.width() / 4);
                        }
                    }
                } else
                {
                    painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 4, buttonRect.width() / 4);
                }
            }
            painter->restore();
            return;
        }
        default: {
            break;
        }
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int ElaRadioButtonStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const {
    switch (metric) {
        case QStyle::PM_ExclusiveIndicatorWidth: {
            return 20;
        }
        case QStyle::PM_ExclusiveIndicatorHeight: {
            return 20;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}
