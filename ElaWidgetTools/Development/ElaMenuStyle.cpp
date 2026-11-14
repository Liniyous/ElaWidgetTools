#include "ElaMenuStyle.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaMenu.h"
#include "ElaTheme.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

ElaMenuStyle::ElaMenuStyle(QStyle* style)
{
    _pMenuItemHeight = 32;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaMenuStyle::~ElaMenuStyle()
{
}

void ElaMenuStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelMenu:
    {
        // 高性能阴影
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        const QMargins margins = eTheme->getShadowMargins();
        eTheme->drawEffectShadow(painter, option->rect, margins, 6);
        // 背景绘制
        const QRect foregroundRect = option->rect.marginsRemoved(margins);
        painter->setPen(ElaThemeColor(_themeMode, BasicBorder));
        painter->setBrush(ElaColorAnimation::getColor(widget, ElaThemeColor(_themeMode, PopupBase)));
        painter->drawRoundedRect(foregroundRect, 6, 6);
        painter->restore();
        return;
    }
    case QStyle::PE_FrameMenu:
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

void ElaMenuStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_MenuItem:
    {
        //内容绘制 区分类型
        if (const QStyleOptionMenuItem* mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option))
        {
            if (mopt->menuItemType == QStyleOptionMenuItem::Separator)
            {
                QRect separatorRect = mopt->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
                painter->setPen(Qt::NoPen);
                painter->setBrush(ElaThemeColor(_themeMode, BasicBaseLine));
                painter->drawRoundedRect(QRectF(separatorRect.x() + separatorRect.width() * 0.055, separatorRect.center().y(), separatorRect.width() - separatorRect.width() * 0.11, 1.5), 1, 1);
                painter->restore();
                return;
            }
            else
            {
                QRect menuRect = mopt->rect;
                qreal contentPadding = menuRect.width() * 0.055;
                qreal textLeftSpacing = 8;
                painter->save();
                painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
                const auto* menuWidget = qobject_cast<const QMenu*>(widget);
                const QAction* backgroundAction = menuWidget ? menuWidget->actionAt(menuRect.center()) : nullptr;
                const bool isHighlighted = mopt->state.testFlag(QStyle::State_Enabled) && (mopt->state.testFlag(QStyle::State_MouseOver) || mopt->state.testFlag(QStyle::State_Selected));
                const QRect hoverRect = menuRect.adjusted(0, 2, 0, -2);
                painter->setPen(Qt::NoPen);
                painter->setBrush(ElaColorAnimation::getColor(widget, isHighlighted ? ElaThemeColor(_themeMode, BasicHoverAlpha) : QColor(Qt::transparent), "MenuItem", {}, backgroundAction));
                painter->drawRoundedRect(hoverRect, 5, 5);
                const QColor foregroundColor = !mopt->state.testFlag(QStyle::State_Enabled) ? QColor(Qt::gray)
                    : mopt->checked                                                         ? ElaThemeColor(_themeMode, PrimaryNormal)
                                                                                            : ElaThemeColor(_themeMode, BasicText);
                const QIcon menuIcon = mopt->icon;
                QString iconText;
                if (backgroundAction && qobject_cast<const ElaMenu*>(widget))
                {
                    iconText = backgroundAction->property("ElaIconType").toString();
                }
                if (iconText.isEmpty() && menuIcon.isNull() && mopt->checked)
                {
                    iconText = QChar(ElaIconType::Check);
                }
                if (!iconText.isEmpty())
                {
                    painter->save();
                    painter->setPen(foregroundColor);
                    QFont iconFont("ElaAwesome");
                    iconFont.setPixelSize(_pMenuItemHeight * 0.57);
                    painter->setFont(iconFont);
                    painter->drawText(QRectF(menuRect.x() + contentPadding, menuRect.y(), _iconWidth, menuRect.height()), Qt::AlignCenter, iconText);
                    painter->restore();
                }
                else if (!menuIcon.isNull())
                {
                    painter->drawPixmap(QRect(menuRect.x() + contentPadding, menuRect.center().y() - _iconWidth / 2, _iconWidth, _iconWidth), menuIcon.pixmap(_iconWidth, _iconWidth));
                }
                //文字和快捷键绘制
                if (!mopt->text.isEmpty())
                {
                    QStringList textList = mopt->text.split("\t");
                    painter->setPen(foregroundColor);

                    painter->drawText(QRectF(menuRect.x() + (_isAnyoneItemHasIcon ? contentPadding + textLeftSpacing : 0) + _iconWidth, menuRect.y(), menuRect.width(), menuRect.height()), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, textList[0]);
                    if (textList.count() > 1)
                    {
                        painter->drawText(QRectF(menuRect.x() + contentPadding + _iconWidth + textLeftSpacing, menuRect.y(), menuRect.width() - (contentPadding * 2 + _iconWidth + textLeftSpacing), menuRect.height()), Qt::AlignRight | Qt::AlignVCenter | Qt::TextSingleLine, textList[1]);
                    }
                }
                //展开图标
                if (mopt->menuItemType == QStyleOptionMenuItem::SubMenu)
                {
                    painter->save();
                    painter->setPen(foregroundColor);
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(eApp->getFontPixelSize() + 5);
                    painter->setFont(iconFont);
                    painter->drawText(QRect(menuRect.right() - 25, menuRect.y(), 25, menuRect.height()), Qt::AlignVCenter, QChar(ElaIconType::AngleRight));
                    painter->restore();
                }
                painter->restore();
            }
        }

        return;
    }
    case QStyle::CE_MenuEmptyArea:
    {
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

int ElaMenuStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_SmallIconSize:
    {
        //图标宽度
        _iconWidth = _pMenuItemHeight * 0.7;
        return _iconWidth;
    }
    case QStyle::PM_MenuPanelWidth:
    {
        // 阴影通过内容边距预留 此处只保留原有的菜单内边距
        return 3;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

QSize ElaMenuStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case QStyle::CT_MenuItem:
    {
        if (const QStyleOptionMenuItem* mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option))
        {
            if (mopt->menuItemType == QStyleOptionMenuItem::Separator)
            {
                break;
            }
            QSize menuItemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            const ElaMenu* menu = dynamic_cast<const ElaMenu*>(widget);
            if (menu->isHasIcon() || mopt->menuHasCheckableItems)
            {
                _isAnyoneItemHasIcon = true;
            }
            if (menu->isHasChildMenu())
            {
                return QSize(menuItemSize.width() + 20, _pMenuItemHeight);
            }
            else
            {
                return QSize(menuItemSize.width(), _pMenuItemHeight);
            }
        }
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}
