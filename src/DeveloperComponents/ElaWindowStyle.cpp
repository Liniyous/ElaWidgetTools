#include "ElaWindowStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

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
    case QStyle::PE_PanelButtonTool:
    {
        if (option->state.testFlag(QStyle::State_Enabled))
        {
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            if (option->state.testFlag(QStyle::State_Sunken))
            {
                painter->setBrush(ElaThemeColor(_themeMode, TabBarPanelButtonToolHover));
            }
            else if (option->state.testFlag(QStyle::State_MouseOver))
            {
                painter->setBrush(ElaThemeColor(_themeMode, TabBarPanelButtonToolPress));
            }
            else
            {
                painter->setBrush(Qt::transparent);
            }
            painter->drawRect(option->rect);
            painter->restore();
        }
        return;
    }
    case QStyle::PE_IndicatorArrowLeft:
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(option->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, WindowText) : ElaThemeColor(_themeMode, WindowTextDisable));
        // 左三角
        int sideLength = 10;
        QRect indicatorRect = option->rect;
        QPainterPath path;
        path.moveTo(indicatorRect.center().x() - qCos(30 * M_PI / 180.0) * sideLength / 2, indicatorRect.center().y());
        path.lineTo(indicatorRect.center().x() + qCos(30 * M_PI / 180.0) * sideLength / 2, indicatorRect.center().y() - sideLength / 2);
        path.lineTo(indicatorRect.center().x() + qCos(30 * M_PI / 180.0) * sideLength / 2, indicatorRect.center().y() + sideLength / 2);
        path.closeSubpath();
        painter->drawPath(path);
        painter->restore();
        return;
    }
    case QStyle::PE_IndicatorArrowRight:
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(option->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, WindowText) : ElaThemeColor(_themeMode, WindowTextDisable));
        // 右三角
        int sideLength = 10;
        QRect indicatorRect = option->rect;
        QPainterPath path;
        path.moveTo(indicatorRect.center().x() - qCos(30 * M_PI / 180.0) * sideLength / 2, indicatorRect.center().y() - sideLength / 2);
        path.lineTo(indicatorRect.center().x() + qCos(30 * M_PI / 180.0) * sideLength / 2, indicatorRect.center().y());
        path.lineTo(indicatorRect.center().x() - qCos(30 * M_PI / 180.0) * sideLength / 2, indicatorRect.center().y() + sideLength / 2);
        path.closeSubpath();
        painter->drawPath(path);
        painter->restore();
        return;
    }
    case QStyle::PE_IndicatorTabTear:
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
    // qDebug() << element << option->rect;
    switch (element)
    {
    case QStyle::CE_RubberBand:
    {
        // 预览颜色
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
        // 背景绘制
        if (const QStyleOptionTab* topt = qstyleoption_cast<const QStyleOptionTab*>(option))
        {
            QRect tabRect = topt->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            painter->setPen(Qt::NoPen);
            if (topt->state.testFlag(QStyle::State_Selected))
            {
                // 选中背景绘制
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
            // 间隔符绘制
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
        // 文字绘制
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
