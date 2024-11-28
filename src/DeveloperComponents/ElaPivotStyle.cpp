#include "ElaPivotStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaTheme.h"
ElaPivotStyle::ElaPivotStyle(QStyle* style)
{
    _pCurrentIndex = -1;
    _pPivotSpacing = 5;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaPivotStyle::~ElaPivotStyle()
{
}

void ElaPivotStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelItemViewRow:
    {
        return;
    }
    case QStyle::PE_Widget:
    {
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void ElaPivotStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        // viewport视口外的其他区域背景
        return;
    }
    case QStyle::CE_ItemViewItem:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            // 内容绘制
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
            QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);
            textRect.adjust(0, 0, 0, -5);
            // 文字绘制
            if (!vopt->text.isEmpty())
            {
                if (_pPressIndex == vopt->index)
                {
                    painter->setPen(ElaThemeColor(_themeMode, BasicTextPress));
                }
                else
                {
                    if (_pCurrentIndex == vopt->index.row() || vopt->state.testFlag(QStyle::State_MouseOver))
                    {
                        painter->setPen(ElaThemeColor(_themeMode, BasicText));
                    }
                    else
                    {
                        painter->setPen(ElaThemeColor(_themeMode, BasicTextNoFocus));
                    }
                }
                painter->drawText(textRect, Qt::AlignCenter, vopt->text);
            }
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

int ElaPivotStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_FocusFrameHMargin:
    {
        return _pPivotSpacing;
    }
    default:
    {
        break;
    }
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}

const QColor& ElaPivotStyle::getMarkColor()
{
    return ElaThemeColor(_themeMode, PrimaryNormal);
}
