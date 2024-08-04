#include "ElaWindowStyle.h"

#include <QPainter>
#include <QStyleOption>

#include "ElaTheme.h"
ElaWindowStyle::ElaWindowStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaWindowStyle::~ElaWindowStyle()
{
}

void ElaWindowStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_FrameTabBarBase:
    {
        return;
    }
    case QStyle::PE_IndicatorDockWidgetResizeHandle:
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(QPen(ElaThemeColor(_themeMode, WindowBaseLine), 2));
        QRectF handleRect = option->rect;
        if (option->state.testFlag(QStyle::State_Horizontal))
        {
            painter->drawLine(handleRect.x(), handleRect.center().y(), handleRect.right(), handleRect.center().y());
        }
        else
        {
            painter->drawLine(handleRect.center().x(), handleRect.y(), handleRect.center().x(), handleRect.bottom());
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

void ElaWindowStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    //qDebug() << element << option->rect;
    switch (element)
    {
    case QStyle::CE_RubberBand:
    {
        //预览颜色
        QRect rubberBandRect = option->rect;
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(ElaThemeColor(_themeMode, WindowRubberBand));
        painter->drawRect(rubberBandRect);
        painter->restore();
        return;
    }
    case QStyle::CE_TabBarTabShape:
    {
        //背景绘制
        if (const QStyleOptionTab* topt = qstyleoption_cast<const QStyleOptionTab*>(option))
        {
            QRect tabRect = topt->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            painter->setPen(Qt::NoPen);
            if (topt->state.testFlag(QStyle::State_Selected))
            {
                //选中背景绘制
                painter->setBrush(ElaThemeColor(_themeMode, WindowTabBarSelected));
            }
            else
            {
                if (topt->state.testFlag(QStyle::State_MouseOver))
                {
                    painter->setBrush(ElaThemeColor(_themeMode, WindowTabBarHover));
                }
                else
                {
                    painter->setBrush(ElaThemeColor(_themeMode, WindowTabBarBase));
                }
            }
            painter->drawRect(tabRect);
            //间隔符绘制
            if (topt->position != QStyleOptionTab::End)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(ElaThemeColor(_themeMode, WindowTabBarSeparator));
                painter->drawRoundedRect(QRectF(tabRect.right() - 3, tabRect.y() + 7, 3, tabRect.height() - 14), 2, 2);
            }
            painter->restore();
        }
        return;
    }
    case QStyle::CE_TabBarTabLabel:
    {
        //文字绘制
        if (const QStyleOptionTab* topt = qstyleoption_cast<const QStyleOptionTab*>(option))
        {
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            painter->setPen(ElaThemeColor(_themeMode, WindowText));
            painter->drawText(topt->rect, Qt::AlignCenter, topt->text);
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
