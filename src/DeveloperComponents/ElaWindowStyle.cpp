#include "ElaWindowStyle.h"

#include <QPainter>
#include <QStyleOption>

#include "ElaApplication.h"
ElaWindowStyle::ElaWindowStyle(QStyle* style)
{
    _themeMode = eApp->getThemeMode();
    connect(eApp, &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { _themeMode = themeMode; });
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
        painter->setPen(Qt::NoPen);
        QRectF handleRect = option->rect;
        if (option->state.testFlag(QStyle::State_MouseOver))
        {
            painter->setBrush(QColor(0x6F, 0xAA, 0xDE));
        }
        else
        {
            painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0x0E, 0x6F, 0xC3) : QColor(0x4C, 0xA0, 0xE0));
        }
        painter->drawRect(handleRect);
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
        painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xFF, 0xFF, 0xFF, 128) : QColor(0x85, 0x85, 0x85, 128));
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
                painter->setBrush(_themeMode == ElaApplicationType::Light ? Qt::white : QColor(0x2D, 0x2D, 0x2D));
            }
            else
            {
                if (topt->state.testFlag(QStyle::State_MouseOver))
                {
                    painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xFC, 0xF8, 0xFF) : QColor(0x27, 0x27, 0x27));
                }
                else
                {
                    painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xF5, 0xF1, 0xF8) : QColor(0x1A, 0x1A, 0x1A));
                }
            }
            painter->drawRect(tabRect);
            //间隔符绘制
            if (topt->position != QStyleOptionTab::End)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0x0E, 0x6F, 0xC3) : QColor(0x4C, 0xA0, 0xE0));
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
            painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
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
