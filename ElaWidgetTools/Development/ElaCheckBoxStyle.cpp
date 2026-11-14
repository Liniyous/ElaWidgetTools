#include "ElaCheckBoxStyle.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaControlStyle.h"
#include "ElaTheme.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

ElaCheckBoxStyle::ElaCheckBoxStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaCheckBoxStyle::~ElaCheckBoxStyle()
{
}

void ElaCheckBoxStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    // qDebug() << element << option->rect;
    switch (element)
    {
    case QStyle::CE_CheckBox:
    {
        if (const QStyleOptionButton* bopt = qstyleoption_cast<const QStyleOptionButton*>(option))
        {
            bool isEnabled = bopt->state.testFlag(QStyle::State_Enabled);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            QRect checkRect = subElementRect(SE_CheckBoxIndicator, bopt, widget);
            checkRect.adjust(1, 1, -1, -1);
            const bool isChecked = bopt->state.testFlag(QStyle::State_On) || bopt->state.testFlag(QStyle::State_NoChange);
            const QColor background = ElaControlStyle::getIndicatorBackgroundColor(_themeMode, bopt->state, isChecked);
            painter->setPen(isChecked ? QPen(Qt::NoPen) : QPen(ElaThemeColor(_themeMode, BasicBorderDeep)));
            painter->setBrush(ElaColorAnimation::getColor(widget, background));
            painter->drawRoundedRect(checkRect, 2, 2);
            //图标绘制
            painter->setPen(ElaThemeColor(_themeMode, BasicTextInvert));
            if (bopt->state.testFlag(QStyle::State_On))
            {
                painter->save();
                QFont iconFont = QFont("ElaAwesome");
                iconFont.setPixelSize(checkRect.height() * 0.75);
                painter->setFont(iconFont);
                painter->drawText(checkRect, Qt::AlignCenter, QChar(ElaIconType::Check));
                painter->restore();
            }
            else if (bopt->state.testFlag(QStyle::State_NoChange))
            {
                QLine checkLine(checkRect.x() + 3, checkRect.center().y(), checkRect.right() - 3, checkRect.center().y());
                painter->drawLine(checkLine);
            }
            //文字绘制
            painter->setPen(isEnabled ? ElaThemeColor(_themeMode, BasicText) : ElaThemeColor(_themeMode, BasicTextDisable));
            QRect textRect = subElementRect(SE_CheckBoxContents, bopt, widget);
            painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}

int ElaCheckBoxStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_IndicatorWidth:
    case QStyle::PM_IndicatorHeight:
    {
        return widget ? widget->fontMetrics().height() : QProxyStyle::pixelMetric(metric, option, widget);
    }
    case QStyle::PM_CheckBoxLabelSpacing:
    {
        return _labelSpacing;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

QRect ElaCheckBoxStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const
{
    QRect subRect = QProxyStyle::subElementRect(element, option, widget);
    if (!widget)
    {
        return subRect;
    }
    if (element == SE_CheckBoxIndicator)
    {
        const int indicatorSize = widget->fontMetrics().height();
        subRect.setSize(QSize(indicatorSize, indicatorSize));
    }
    else if (element == SE_CheckBoxContents)
    {
        const QRect indicatorRect = subElementRect(SE_CheckBoxIndicator, option, widget);
        subRect.setLeft(indicatorRect.right() + _labelSpacing);
    }
    return subRect;
}
