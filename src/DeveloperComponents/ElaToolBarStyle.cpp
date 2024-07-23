#include "ElaToolBarStyle.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "ElaTheme.h"
#include "ElaToolBar.h"
ElaToolBarStyle::ElaToolBarStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaToolBarStyle::~ElaToolBarStyle()
{
}

void ElaToolBarStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelButtonTool:
    {
        return;
    }
    case QStyle::PE_IndicatorToolBarHandle:
    {
        //拖动虚线
        QRect handleRect = option->rect;
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QPen handlePen(Qt::DotLine);
        handlePen.setColor(ElaThemeColor(_themeMode, ToolBarHandle));
        handlePen.setWidthF(1.8);
        painter->setPen(handlePen);
        const ElaToolBar* toolBar = dynamic_cast<const ElaToolBar*>(widget);
        if (toolBar->orientation() == Qt::Vertical)
        {
            painter->drawLine(handleRect.x() + handleRect.width() * 0.1, handleRect.center().y(), handleRect.right() - handleRect.width() * 0.1, handleRect.center().y());
        }
        else
        {
            painter->drawLine(handleRect.center().x(), handleRect.y() + handleRect.height() * 0.1, handleRect.center().x(), handleRect.bottom() - handleRect.height() * 0.1);
        }
        painter->restore();
        return;
    }
    case QStyle::PE_IndicatorToolBarSeparator:
    {
        //间隔符绘制
        QRect separatorRect = option->rect;
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(ElaThemeColor(_themeMode, ToolBarSeparator));
        const ElaToolBar* toolBar = dynamic_cast<const ElaToolBar*>(widget);
        if (toolBar->orientation() == Qt::Vertical)
        {
            painter->drawRect(QRectF(separatorRect.x() + separatorRect.width() * 0.1, separatorRect.center().y() - 0.9, separatorRect.width() * 0.8, 1.8));
        }
        else
        {
            painter->drawRect(QRectF(separatorRect.center().x() - 0.9, separatorRect.y() + separatorRect.height() * 0.1, 1.8, separatorRect.height() * 0.8));
        }
        painter->restore();
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void ElaToolBarStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ToolButtonLabel:
    {
        //展开按钮
        if (const QStyleOptionToolButton* topt = qstyleoption_cast<const QStyleOptionToolButton*>(option))
        {
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            painter->setPen(Qt::NoPen);
            if (topt->state.testFlag(QStyle::State_Enabled) && topt->state.testFlag(QStyle::State_MouseOver))
            {
                painter->setBrush(ElaThemeColor(_themeMode, ToolBarExpandButtonHover));
                painter->drawRect(topt->rect);
            }
            //展开图标
            painter->setPen(!topt->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, WindowTextDisable) : ElaThemeColor(_themeMode, WindowText));
            QFont iconFont = QFont("ElaAwesome");
            iconFont.setPixelSize(18);
            painter->setFont(iconFont);
            painter->drawText(topt->rect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::AngleRight));
            painter->restore();
        }
        return;
    }
    case QStyle::CE_ToolBar:
    {
        //背景和边框线绘制
        if (const QStyleOptionToolBar* topt = qstyleoption_cast<const QStyleOptionToolBar*>(option))
        {
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            if (topt->state.testFlag(QStyle::State_Window))
            {
                painter->setPen(ElaThemeColor(_themeMode, ToolBarWindowBorder));
            }
            else
            {
                painter->setPen(Qt::NoPen);
            }
            painter->setBrush(ElaThemeColor(_themeMode, ToolBarWindowBase));
            painter->drawRect(topt->rect);
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

int ElaToolBarStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_ToolBarExtensionExtent:
    {
        return 16;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}
