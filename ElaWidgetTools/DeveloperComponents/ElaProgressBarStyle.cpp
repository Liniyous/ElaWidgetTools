#include "ElaProgressBarStyle.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "ElaTheme.h"
ElaProgressBarStyle::ElaProgressBarStyle(QStyle* style)
{
    setProperty("busyStartValue", 0);
    setProperty("busyEndValue", 0);
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaProgressBarStyle::~ElaProgressBarStyle()
{
}

void ElaProgressBarStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ProgressBarLabel:
    {
        if (const QStyleOptionProgressBar* popt = qstyleoption_cast<const QStyleOptionProgressBar*>(option))
        {
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            painter->setPen(ElaThemeColor(_themeMode, BasicText));
            painter->drawText(popt->rect, Qt::AlignCenter, popt->text);
            painter->restore();
        }
        return;
    }
    case QStyle::CE_ProgressBarGroove:
    {
        //背景轨道
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(ElaThemeColor(_themeMode, BasicChute));
        painter->drawRoundedRect(option->rect, 3, 3);
        painter->restore();
        return;
    }
    case QStyle::CE_ProgressBarContents:
    {
        //滑块
        const QStyleOptionProgressBar* popt = qstyleoption_cast<const QStyleOptionProgressBar*>(option);
        if (!popt)
        {
            break;
        }
        QRect contentRect = popt->rect;
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
        const bool inverted = popt->invertedAppearance;
        bool reverse = popt->direction == Qt::RightToLeft;
        if (inverted)
        {
            reverse = !reverse;
        }
        if (popt->minimum == 0 && popt->maximum == 0)
        {
            //忙碌动画
            int startValue = this->property("busyStartValue").toInt();
            if (startValue < 0)
            {
                startValue = 0;
            }
            int endValue = this->property("busyEndValue").toInt();
            if (popt->state & QStyle::State_Horizontal)
            {
                if (endValue > contentRect.width())
                {
                    endValue = contentRect.width();
                }
                int busyWidth = endValue - startValue;
                if (reverse)
                {
                    painter->translate((contentRect.x() + contentRect.width()) / 2, contentRect.y());
                    painter->rotate(180);
                    painter->translate(-(contentRect.x() + contentRect.width()) / 2, -contentRect.y() - contentRect.height() * 0.90);
                }
                painter->drawRoundedRect(QRectF(startValue, contentRect.y(), busyWidth, contentRect.height()), 3, 3);
            }
            else
            {
                if (endValue > contentRect.height())
                {
                    endValue = contentRect.height();
                }
                int busyHeight = endValue - startValue;
                painter->drawRoundedRect(QRectF(contentRect.x(), contentRect.height() - endValue, contentRect.width(), busyHeight), 3, 3);
            }
        }
        else
        {
            qreal ratio = popt->progress / (qreal)(popt->maximum - popt->minimum);
            if (popt->state & QStyle::State_Horizontal)
            {
                if (reverse)
                {
                    painter->translate((contentRect.x() + contentRect.width()) / 2, contentRect.y());
                    painter->rotate(180);
                    painter->translate(-(contentRect.x() + contentRect.width()) / 2, -contentRect.y() - contentRect.height() * 0.90);
                }
                painter->drawRoundedRect(QRectF(contentRect.x(), contentRect.y(), contentRect.width() * ratio, contentRect.height()), 3, 3);
            }
            else
            {
                painter->drawRoundedRect(QRectF(contentRect.x(), contentRect.y() + contentRect.height() * (1 - ratio), contentRect.width(), contentRect.height() * ratio), 3, 3);
            }
        }
        painter->restore();
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

QRect ElaProgressBarStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::SE_ProgressBarGroove:
    case QStyle::SE_ProgressBarContents:
    {
        QRect textRect = subElementRect(SE_ProgressBarLabel, option, widget);
        const QStyleOptionProgressBar* popt = qstyleoption_cast<const QStyleOptionProgressBar*>(option);
        if (!popt)
        {
            break;
        }
        QRect contentRect = popt->rect;
        int width = contentRect.width();
        int height = contentRect.height();
        if (popt->state & QStyle::State_Horizontal)
        {
            contentRect.setTop(contentRect.top() + height * 0.45);
            contentRect.setBottom(contentRect.bottom() - height * 0.30);
            if (!(popt->minimum == 0 && popt->maximum == 0))
            {
                contentRect.setWidth(contentRect.width() - textRect.width());
            }
        }
        else
        {
            contentRect.setLeft(contentRect.left() + width * 0.375);
            contentRect.setRight(contentRect.right() - width * 0.375);
        }
        return contentRect;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::subElementRect(element, option, widget);
}
