#include "ElaTabBarStyle.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaControlStyle.h"
#include "ElaTheme.h"

#include <QFontMetricsF>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

ElaTabBarStyle::ElaTabBarStyle(QStyle* style)
{
    _pTabSize = QSize(220, 35);
    _pIsActive = true;
    _pIsPinned = false;
    _pIsTabWidthAdaptive = false;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaTabBarStyle::~ElaTabBarStyle()
{
}

void ElaTabBarStyle::drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const
{
    switch (pe)
    {
    case QStyle::PE_FrameTabBarBase:
    {
        //底边线
        return;
    }
    case QStyle::PE_IndicatorArrowLeft:
    {
        return;
    }
    case QStyle::PE_IndicatorArrowRight:
    {
        return;
    }
    case QStyle::PE_PanelButtonTool:
    {
        //左侧隐藏图标
        return;
    }
    case QStyle::PE_IndicatorTabTear:
    {
        return;
    }
    case QStyle::PE_IndicatorTabClose:
    {
        p->save();
        p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        p->setPen(Qt::NoPen);
        p->setBrush(ElaColorAnimation::getColor(w, opt->state.testFlag(QStyle::State_MouseOver) ? ElaThemeColor(_themeMode, BasicHoverAlpha) : QColor(Qt::transparent), "TabClose"));
        p->drawRoundedRect(opt->rect, 2, 2);
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(eApp->getFontPixelSize() + (_pIsPinned ? 0 : 3));
        p->setFont(iconFont);
        p->setPen(ElaThemeColor(_themeMode, BasicText));
        p->drawText(opt->rect, Qt::AlignCenter, QChar(_pIsPinned ? ElaIconType::Thumbtack : ElaIconType::Xmark));
        p->restore();
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(pe, opt, p, w);
}

void ElaTabBarStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_TabBarTab:
    {
        if (const QStyleOptionTab* topt = qstyleoption_cast<const QStyleOptionTab*>(option))
        {
            drawControl(CE_TabBarTabShape, topt, painter, widget);
            drawControl(CE_TabBarTabLabel, topt, painter, widget);
            return;
        }
    }
    case QStyle::CE_TabBarTabShape:
    {
        //背景绘制
        if (const QStyleOptionTab* topt = qstyleoption_cast<const QStyleOptionTab*>(option))
        {
            const QRectF tabRect = QRectF(topt->rect).adjusted(2, 1, -2, -1);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            painter->setPen(Qt::NoPen);
            const QColor background = _pIsActive && topt->state.testFlag(QStyle::State_Selected)
                ? (topt->state.testFlag(QStyle::State_Sunken) ? ElaThemeColor(_themeMode, BasicPressAlpha) : ElaThemeColor(_themeMode, BasicHoverAlpha))
                : topt->state.testFlag(QStyle::State_MouseOver) ? ElaThemeColor(_themeMode, BasicHoverAlpha)
                                                                : QColor(Qt::transparent);
            painter->setBrush(ElaControlStyle::getTabBackgroundColor(widget, topt, background));
            if (_pIsActive && topt->state.testFlag(QStyle::State_Selected))
            {
                const QRectF selectedRect = QRectF(topt->rect).adjusted(0, 1, 0, 0);
                const qreal radius = qMin<qreal>(8, selectedRect.height() / 3);
                const qreal left = selectedRect.left();
                const qreal right = selectedRect.right();
                const qreal top = selectedRect.top();
                const qreal bottom = selectedRect.bottom();
                QPainterPath path;
                path.moveTo(left, bottom);
                path.arcTo(QRectF(left - radius, bottom - 2 * radius, 2 * radius, 2 * radius), -90, 90);
                path.lineTo(left + radius, top + radius);
                path.arcTo(QRectF(left + radius, top, 2 * radius, 2 * radius), 180, -90);
                path.lineTo(right - 2 * radius, top);
                path.arcTo(QRectF(right - 3 * radius, top, 2 * radius, 2 * radius), 90, -90);
                path.lineTo(right - radius, bottom - radius);
                path.arcTo(QRectF(right - radius, bottom - 2 * radius, 2 * radius, 2 * radius), 180, 90);
                path.closeSubpath();
                painter->drawPath(path);
            }
            else
            {
                painter->drawRoundedRect(tabRect, 6, 6);
            }
            painter->restore();
            return;
        }
    }
    case QStyle::CE_TabBarTabLabel:
    {
        //文字和图标绘制
        if (const QStyleOptionTab* topt = qstyleoption_cast<const QStyleOptionTab*>(option))
        {
            QRect textRect = subElementRect(QStyle::SE_TabBarTabText, topt, widget);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            //图标绘制
            QIcon icon = topt->icon;
            if (!icon.isNull())
            {
                const QRect iconRect(textRect.left() - topt->iconSize.width() - 6, topt->rect.top() + (topt->rect.height() - topt->iconSize.height()) / 2, topt->iconSize.width(), topt->iconSize.height());
                icon.paint(painter, iconRect, Qt::AlignCenter,
                           topt->state.testFlag(State_Enabled) ? QIcon::Normal : QIcon::Disabled,
                           topt->state.testFlag(State_Selected) ? QIcon::On : QIcon::Off);
            }
            //文字绘制
            const bool selected = _pIsActive && topt->state.testFlag(QStyle::State_Selected);
            const bool activeGroup = !widget->property("ElaActiveGroup").isValid() || widget->property("ElaActiveGroup").toBool();
            painter->setPen(selected && activeGroup ? ElaThemeColor(_themeMode, PrimaryNormal) : ElaThemeColor(_themeMode, BasicText));
            const auto text = painter->fontMetrics().elidedText(topt->text.simplified(), Qt::ElideRight, qMax(0, textRect.width()));
            painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, text);
            painter->restore();
            return;
        }
    }
    default:
    {
        break;
    }
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}

QSize ElaTabBarStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case CT_TabBarTab:
    {
        if (_pIsTabWidthAdaptive)
        {
            if (const auto* tab = qstyleoption_cast<const QStyleOptionTab*>(option))
            {
                // 按浮点字宽向上取整预留文本空间 避免整数四舍五入舍弃小数后触发右侧省略
                const int textWidth = qCeil(QFontMetricsF(tab->fontMetrics).horizontalAdvance(tab->text.simplified()));
                int width = 30 + textWidth;
                if (!tab->icon.isNull())
                {
                    width += tab->iconSize.width() + 6;
                }
                for (const auto& button: {tab->leftButtonSize, tab->rightButtonSize})
                {
                    if (!button.isEmpty())
                    {
                        width += button.width() + 6;
                    }
                }
                return {qMin(width, _pTabSize.width()), _pTabSize.height()};
            }
        }
        return _pTabSize;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

QRect ElaTabBarStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::SE_TabBarTabText:
    case QStyle::SE_TabBarTabLeftButton:
    case QStyle::SE_TabBarTabRightButton:
    {
        if (const auto* tab = qstyleoption_cast<const QStyleOptionTab*>(option))
        {
            QRect rect = tab->rect.adjusted(15, 0, -15, 0);
            if (element != QStyle::SE_TabBarTabText)
            {
                const auto size = element == QStyle::SE_TabBarTabLeftButton ? tab->leftButtonSize : tab->rightButtonSize;
                return {element == QStyle::SE_TabBarTabLeftButton ? rect.left() : rect.right() - size.width() + 1,
                        rect.top() + (rect.height() - size.height()) / 2, size.width(), size.height()};
            }
            if (!tab->leftButtonSize.isEmpty())
            {
                rect.adjust(tab->leftButtonSize.width() + 6, 0, 0, 0);
            }
            if (!tab->icon.isNull())
            {
                rect.adjust(tab->iconSize.width() + 6, 0, 0, 0);
            }
            if (!tab->rightButtonSize.isEmpty())
            {
                rect.adjust(0, 0, -tab->rightButtonSize.width() - 6, 0);
            }
            return rect;
        }
        break;
    }
    case QStyle::SE_TabBarScrollLeftButton:
    case QStyle::SE_TabBarScrollRightButton:
    case QStyle::SE_TabBarTearIndicatorLeft:
    case QStyle::SE_TabBarTearIndicatorRight:
    {
        return QRect();
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::subElementRect(element, option, widget);
}
