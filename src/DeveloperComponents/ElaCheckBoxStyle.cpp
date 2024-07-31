#include "ElaCheckBoxStyle.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "ElaTheme.h"
ElaCheckBoxStyle::ElaCheckBoxStyle(QStyle* style)
{
    _pCheckIndicatorWidth = 21;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaCheckBoxStyle::~ElaCheckBoxStyle()
{
}

void ElaCheckBoxStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    //qDebug() << element << option->rect;
    QProxyStyle::drawPrimitive(element, option, painter, widget);
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
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            QRect checkBoxRect = bopt->rect;
            QRect checkRect(checkBoxRect.x(), checkBoxRect.y(), _pCheckIndicatorWidth, _pCheckIndicatorWidth);
            checkRect.adjust(1, 1, -1, -1);
            //复选框绘制
            painter->setPen(Qt::NoPen);
            if (bopt->state.testFlag(QStyle::State_On) || bopt->state.testFlag(QStyle::State_NoChange))
            {
                painter->setPen(ElaThemeColor(_themeMode, CheckBoxCheckedBorder));
                if (bopt->state.testFlag(QStyle::State_Sunken))
                {
                    painter->setBrush(ElaThemeColor(_themeMode, CheckBoxCheckedPress));
                }
                else
                {
                    if (bopt->state.testFlag(QStyle::State_MouseOver))
                    {
                        painter->setBrush(ElaThemeColor(_themeMode, CheckBoxCheckedHover));
                    }
                    else
                    {
                        painter->setBrush(ElaThemeColor(_themeMode, CheckBoxCheckedBase));
                    }
                }
            }
            else
            {
                if (bopt->state.testFlag(QStyle::State_Sunken))
                {
                    painter->setPen(ElaThemeColor(_themeMode, CheckBoxPressBorder));
                }
                else
                {
                    painter->setPen(ElaThemeColor(_themeMode, CheckBoxBorder));
                    if (bopt->state.testFlag(QStyle::State_MouseOver))
                    {
                        painter->setBrush(ElaThemeColor(_themeMode, CheckBoxHover));
                    }
                    else
                    {
                        painter->setBrush(ElaThemeColor(_themeMode, CheckBoxBase));
                    }
                }
            }
            painter->drawRoundedRect(checkRect, 2, 2);
            //图标绘制
            painter->setPen(ElaThemeColor(ElaThemeType::Dark, WindowText));
            if (bopt->state.testFlag(QStyle::State_On))
            {
                painter->save();
                QFont iconFont = QFont("ElaAwesome");
                iconFont.setPixelSize(_pCheckIndicatorWidth * 0.75);
                painter->setFont(iconFont);
                painter->drawText(checkRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::Check));
                painter->restore();
            }
            else if (bopt->state.testFlag(QStyle::State_NoChange))
            {
                QLine checkLine(checkRect.x() + 3, checkRect.center().y(), checkRect.right() - 3, checkRect.center().y());
                painter->drawLine(checkLine);
            }
            //文字绘制
            painter->setPen(ElaThemeColor(_themeMode, WindowText));
            QRect textRect(checkRect.right() + 10, checkBoxRect.y(), checkBoxRect.width(), checkBoxRect.height());
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
    // qDebug() << metric << QProxyStyle::pixelMetric(metric, option, widget);
    switch (metric)
    {
    case QStyle::PM_IndicatorWidth:
    {
        return _pCheckIndicatorWidth;
    }
    case QStyle::PM_IndicatorHeight:
    {
        return _pCheckIndicatorWidth;
    }
    case QStyle::PM_CheckBoxLabelSpacing:
    {
        return 10;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}
