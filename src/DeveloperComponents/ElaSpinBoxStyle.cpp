#include "ElaSpinBoxStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionSpinBox>

#include "ElaTheme.h"
ElaSpinBoxStyle::ElaSpinBoxStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaSpinBoxStyle::~ElaSpinBoxStyle()
{
}

void ElaSpinBoxStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
    case QStyle::CC_SpinBox:
    {
        const QStyleOptionSpinBox* sopt = qstyleoption_cast<const QStyleOptionSpinBox*>(option);
        if (!sopt)
        {
            break;
        }
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        //背景
        painter->setPen(Qt::NoPen);
        painter->setBrush(ElaThemeColor(_themeMode, SpinBoxBase));
        painter->drawRoundedRect(sopt->rect, 4, 4);
        //添加按钮
        QRect addLineRect = subControlRect(control, sopt, SC_ScrollBarAddLine, widget);
        if (sopt->activeSubControls == SC_ScrollBarAddLine)
        {
            if (sopt->state & QStyle::State_Sunken && sopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(ElaThemeColor(_themeMode, SpinBoxButtonPress));
            }
            else
            {
                if (sopt->state & QStyle::State_MouseOver)
                {
                    painter->setBrush(ElaThemeColor(_themeMode, SpinBoxButtonHover));
                }
                else
                {
                    painter->setBrush(ElaThemeColor(_themeMode, SpinBoxButtonBase));
                }
            }
        }
        else
        {
            painter->setBrush(ElaThemeColor(_themeMode, SpinBoxButtonBase));
        }
        QPainterPath addLinePath;
        addLinePath.moveTo(addLineRect.topLeft());
        addLinePath.lineTo(addLineRect.bottomLeft());
        addLinePath.lineTo(addLineRect.right() - 4, addLineRect.bottom());
        addLinePath.arcTo(QRectF(addLineRect.right() - 8, addLineRect.bottom() - 8, 8, 8), -90, 90);
        addLinePath.lineTo(addLineRect.right(), addLineRect.y() + 4);
        addLinePath.arcTo(QRectF(addLineRect.right() - 8, addLineRect.y(), 8, 8), 0, 90);
        addLinePath.closeSubpath();
        painter->drawPath(addLinePath);

        //减少按钮
        QRect subLineRect = subControlRect(control, sopt, SC_ScrollBarSubLine, widget);
        if (sopt->activeSubControls == SC_ScrollBarSubLine)
        {
            if (sopt->state & QStyle::State_Sunken && sopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(ElaThemeColor(_themeMode, SpinBoxButtonPress));
            }
            else
            {
                if (sopt->state & QStyle::State_MouseOver)
                {
                    painter->setBrush(ElaThemeColor(_themeMode, SpinBoxButtonHover));
                }
                else
                {
                    painter->setBrush(ElaThemeColor(_themeMode, SpinBoxButtonBase));
                }
            }
        }
        else
        {
            painter->setBrush(ElaThemeColor(_themeMode, SpinBoxButtonBase));
        }
        QPainterPath subLinePath;
        subLinePath.moveTo(subLineRect.topRight());
        subLinePath.lineTo(subLineRect.x() + 4, subLineRect.y());
        subLinePath.arcTo(QRectF(subLineRect.x(), subLineRect.y(), 8, 8), 90, 90);
        subLinePath.lineTo(subLineRect.x(), subLineRect.bottom() - 4);
        subLinePath.arcTo(QRectF(subLineRect.x(), subLineRect.bottom() - 8, 8, 8), 180, 90);
        subLinePath.lineTo(subLineRect.bottomRight());
        subLinePath.closeSubpath();
        painter->drawPath(subLinePath);
        //底边线
        if (sopt->state & QStyle::State_HasFocus)
        {
            painter->setPen(QPen(ElaThemeColor(_themeMode, SpinBoxMarkHasFocus), 2));
            painter->drawLine(subLineRect.right() + 1, subLineRect.bottom() - 2, addLineRect.left() - 1, addLineRect.bottom() - 2);
        }
        else
        {
            painter->setPen(ElaThemeColor(_themeMode, SpinBoxMarkNoFocus));
            painter->drawLine(subLineRect.right() + 1, subLineRect.bottom() - 1, addLineRect.left() - 1, addLineRect.bottom() - 1);
        }

        //添加图标
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(17);
        painter->setFont(iconFont);
        painter->setPen(_themeMode == ElaThemeType::Light ? Qt::black : Qt::white);
        painter->drawText(addLineRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::Plus));
        //减小图标
        painter->drawText(subLineRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::Minus));
        painter->restore();
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QRect ElaSpinBoxStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const
{
    QRect rect = QProxyStyle::subControlRect(cc, opt, sc, widget);
    switch (cc)
    {
    case CC_SpinBox:
    {
        switch (sc)
        {
        case SC_ScrollBarAddLine:
        {
            //增加按钮
            QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
            return QRect(spinBoxRect.width() - spinBoxRect.height(), 0, spinBoxRect.height(), spinBoxRect.height());
        }
        case SC_ScrollBarSubLine:
        {
            //减少按钮
            QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
            return QRect(0, 0, spinBoxRect.height(), spinBoxRect.height());
        }
        case SC_SpinBoxEditField:
        {
            QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
            return QRect(spinBoxRect.height(), 0, spinBoxRect.width() - 2 * spinBoxRect.height(), spinBoxRect.height());
        }
        default:
        {
            break;
        }
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return rect;
}
