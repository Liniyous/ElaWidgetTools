#include "ElaSpinBoxStyle.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaControlStyle.h"
#include "ElaTheme.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOptionSpinBox>

ElaSpinBoxStyle::ElaSpinBoxStyle(QStyle* style)
{
    _pButtonMode = ElaSpinBoxType::Inline;
    _pExpandMarkProgress = 0;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
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
        bool isEnable = sopt->state.testFlag(QStyle::State_Enabled);
        ElaControlStyle::drawInputFrame(painter, sopt->rect, 4, _themeMode, sopt->state, widget, _pExpandMarkProgress);
        //添加按钮
        QRect addLineRect = subControlRect(control, sopt, SC_ScrollBarAddLine, widget);
        //减少按钮
        QRect subLineRect = subControlRect(control, sopt, SC_ScrollBarSubLine, widget);
        const QColor hoverColor = sopt->state.testFlag(QStyle::State_Sunken) ? ElaThemeColor(_themeMode, BasicPressAlpha) : ElaThemeColor(_themeMode, BasicHoverAlpha);
        const bool isHovered = isEnable && sopt->state.testFlag(QStyle::State_MouseOver);
        painter->setPen(Qt::NoPen);
        painter->setBrush(ElaColorAnimation::getColor(widget, isHovered && sopt->activeSubControls.testFlag(SC_ScrollBarAddLine) ? hoverColor : QColor(Qt::transparent), "Increment"));
        painter->drawRoundedRect(addLineRect, 4, 4);
        painter->setBrush(ElaColorAnimation::getColor(widget, isHovered && sopt->activeSubControls.testFlag(SC_ScrollBarSubLine) ? hoverColor : QColor(Qt::transparent), "Decrement"));
        painter->drawRoundedRect(subLineRect, 4, 4);
        //添加图标
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(eApp->getFontPixelSize() + 4);
        painter->setFont(iconFont);
        painter->setPen(isEnable ? ElaThemeColor(_themeMode, BasicText) : ElaThemeColor(_themeMode, BasicTextDisable));
        painter->drawText(addLineRect, Qt::AlignCenter, _pButtonMode == ElaSpinBoxType::PMSide ? QChar(ElaIconType::Plus) : QChar(ElaIconType::AngleUp));
        //减小图标
        painter->drawText(subLineRect, Qt::AlignCenter, _pButtonMode == ElaSpinBoxType::PMSide ? QChar(ElaIconType::Minus) : QChar(ElaIconType::AngleDown));
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
            QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget).adjusted(1, 1, -1, -1);
            switch (_pButtonMode)
            {
            case ElaSpinBoxType::Inline:
            {
                return QRect(spinBoxRect.width() - spinBoxRect.height(), spinBoxRect.y(), spinBoxRect.height(), spinBoxRect.height()).adjusted(3, 4, -3, -5);
            }
            case ElaSpinBoxType::Compact:
            {
                return QRect(spinBoxRect.width() - spinBoxRect.height(), spinBoxRect.y(), spinBoxRect.height(), spinBoxRect.height() / 2).adjusted(3, 4, -3, 0);
            }
            case ElaSpinBoxType::Side:
            case ElaSpinBoxType::PMSide:
            {
                return QRect(spinBoxRect.width() - spinBoxRect.height(), spinBoxRect.y(), spinBoxRect.height(), spinBoxRect.height()).adjusted(3, 4, -3, -5);
            }
            }
        }
        case SC_ScrollBarSubLine:
        {
            //减少按钮
            QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget).adjusted(1, 1, -1, -1);
            switch (_pButtonMode)
            {
            case ElaSpinBoxType::Inline:
            {
                return QRect(spinBoxRect.width() - 2 * spinBoxRect.height(), spinBoxRect.y(), spinBoxRect.height(), spinBoxRect.height()).adjusted(6, 4, 0, -5);
            }
            case ElaSpinBoxType::Compact:
            {
                return QRect(spinBoxRect.width() - spinBoxRect.height(), spinBoxRect.center().y(), spinBoxRect.height(), spinBoxRect.height() / 2).adjusted(3, 0, -3, -5);
            }
            case ElaSpinBoxType::Side:
            case ElaSpinBoxType::PMSide:
            {
                return QRect(spinBoxRect.x(), spinBoxRect.y(), spinBoxRect.height(), spinBoxRect.height()).adjusted(3, 4, -3, -5);
            }
            }
        }
        case SC_SpinBoxEditField:
        {
            QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
            switch (_pButtonMode)
            {
            case ElaSpinBoxType::Inline:
            {
                return {spinBoxRect.x(), spinBoxRect.y(), spinBoxRect.width() - 2 * spinBoxRect.height() + 6, spinBoxRect.height()};
            }
            case ElaSpinBoxType::Compact:
            {
                return {spinBoxRect.x(), spinBoxRect.y(), spinBoxRect.width() - spinBoxRect.height() + 3, spinBoxRect.height()};
            }
            case ElaSpinBoxType::Side:
            case ElaSpinBoxType::PMSide:
            {
                return {spinBoxRect.height(), spinBoxRect.y(), spinBoxRect.width() - 2 * spinBoxRect.height(), spinBoxRect.height()};
            }
            }
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
