#include "ElaLineEditStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaApplication.h"
ElaLineEditStyle::ElaLineEditStyle(QStyle* style)
{
    _themeMode = ElaApplication::getInstance()->getThemeMode();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode)
            { _themeMode = themeMode; });
}

ElaLineEditStyle::~ElaLineEditStyle()
{
}

void ElaLineEditStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case PE_FrameLineEdit:
    {
        if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
        {
        }
        break;
    }
    case PE_PanelLineEdit:
    {
        if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
        {
            QRect lineEditRect = fopt->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            // 边框绘制
            painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xE1, 0xE6, 0xEA) : QColor(0x4C, 0x4C, 0x4C));
            painter->drawRoundedRect(lineEditRect, 6, 6);
            //  背景绘制
            if (fopt->state & QStyle::State_HasFocus)
            {
                painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xFF, 0xFF, 0xFF) : QColor(0x24, 0x24, 0x24));
            }
            else if (fopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xF4, 0xF7, 0xF9) : QColor(0x44, 0x44, 0x44));
            }
            else
            {
                painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xFA, 0xFC, 0xFD) : QColor(0x3E, 0x3E, 0x3E));
            }
            painter->drawRoundedRect(QRectF(lineEditRect.x() + 1.5, lineEditRect.y() + 1.5, lineEditRect.width() - 3, lineEditRect.height() - 3), 6, 6);

            // 底边线绘制
            if (fopt->state & QStyle::State_HasFocus)
            {
                painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0x00, 0x67, 0xC0) : QColor(0x00, 0x67, 0xC0));
            }
            else
            {
                painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0x86, 0x86, 0x86) : QColor(0x86, 0x86, 0x86));
            }
            QPainterPath path;
            path.moveTo(6, lineEditRect.height());
            path.lineTo(lineEditRect.width() - 6, lineEditRect.height());
            path.arcTo(QRectF(lineEditRect.width() - 12, lineEditRect.height() - 12, 12, 12), -90, 45);
            path.lineTo(6 - 3 * sqrt(2), lineEditRect.height() - (6 - 3 * sqrt(2)));
            path.arcTo(QRectF(0, lineEditRect.height() - 12, 12, 12), 270, 45);
            path.closeSubpath();
            painter->drawPath(path);
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
