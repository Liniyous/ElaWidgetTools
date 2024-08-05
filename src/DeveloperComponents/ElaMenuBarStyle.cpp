#include "ElaMenuBarStyle.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "ElaMenuBar.h"
#include "ElaTheme.h"
ElaMenuBarStyle::ElaMenuBarStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaMenuBarStyle::~ElaMenuBarStyle()
{
}

void ElaMenuBarStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelButtonTool:
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

void ElaMenuBarStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ToolButtonLabel:
    {
        if (const QStyleOptionToolButton* topt = qstyleoption_cast<const QStyleOptionToolButton*>(option))
        {
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            painter->setPen(Qt::NoPen);
            if (topt->state.testFlag(QStyle::State_Enabled) && topt->state.testFlag(QStyle::State_MouseOver))
            {
                painter->setBrush(ElaThemeColor(_themeMode, MenuBarExpandButtonHover));
                painter->drawRect(topt->rect);
            }
            //展开图标
            painter->setPen(!topt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : _themeMode == ElaThemeType::Light ? Qt::black
                                                                                                                        : Qt::white);
            QFont iconFont = QFont("ElaAwesome");
            iconFont.setPixelSize(18);
            painter->setFont(iconFont);
            painter->drawText(topt->rect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::AngleRight));
            painter->restore();
        }
        return;
    }
    case QStyle::CE_MenuBarEmptyArea:
    {
        return;
    }
    case QStyle::CE_MenuBarItem:
    {
        if (const QStyleOptionMenuItem* mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option))
        {
            if (mopt->menuItemType == QStyleOptionMenuItem::Separator)
            {
                return;
            }
            int menuBarHeight = widget->height();
            QRect menuItemRect = mopt->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            //背景绘制
            painter->setPen(Qt::NoPen);
            if (mopt->state.testFlag(QStyle::State_Enabled))
            {
                if (mopt->state.testFlag(QStyle::State_Sunken))
                {
                    painter->setBrush(ElaThemeColor(_themeMode, MenuBarItemPress));
                }
                else if (mopt->state.testFlag(QStyle::State_Selected))
                {
                    painter->setBrush(ElaThemeColor(_themeMode, MenuBarItemSelected));
                }
                painter->drawRoundedRect(menuItemRect, 3, 3);
            }
            //图标和文字绘制
            QIcon icon = mopt->icon;
            QString menuText = mopt->text;
            QString iconText;
            menuText = menuText.replace("&", "");
            const ElaMenuBar* menuBar = dynamic_cast<const ElaMenuBar*>(widget);
            if (menuBar)
            {
                QAction* action = menuBar->actionAt(menuItemRect.center());
                if (action)
                {
                    iconText = action->property("ElaIconType").toString();
                }
            }
            if (menuText.isEmpty())
            {
                //图标
                if (!iconText.isEmpty())
                {
                    painter->save();
                    painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, WindowTextDisable) : ElaThemeColor(_themeMode, WindowText));
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(menuBarHeight * 0.7);
                    painter->setFont(iconFont);
                    painter->drawText(menuItemRect, Qt::AlignCenter, iconText);
                    painter->restore();
                }
                else if (!icon.isNull())
                {
                    QRect menuIconRect(menuItemRect.center().x() - menuBarHeight * 0.4, menuItemRect.center().y() - menuBarHeight * 0.4, menuBarHeight * 0.8, menuBarHeight * 0.8);
                    painter->drawPixmap(menuIconRect, icon.pixmap(menuBarHeight * 0.8, menuBarHeight * 0.8));
                }
            }
            else
            {
                //图标 + 文字
                painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, WindowTextDisable) : ElaThemeColor(_themeMode, WindowText));
                if (icon.isNull() && iconText.isEmpty())
                {
                    painter->drawText(menuItemRect, Qt::AlignCenter, menuText);
                }
                else
                {
                    if (!iconText.isEmpty())
                    {
                        QRectF menuIconRect(menuItemRect.x() + menuBarHeight * 0.1 + _menuBarItemMargin, menuBarHeight * 0.1, menuBarHeight * 0.8, menuBarHeight * 0.8);
                        painter->save();
                        QFont iconFont = QFont("ElaAwesome");
                        iconFont.setPixelSize(menuBarHeight * 0.7);
                        painter->setFont(iconFont);
                        painter->drawText(menuIconRect, Qt::AlignCenter, iconText);
                        painter->restore();
                        painter->drawText(QRect(menuIconRect.right(), menuItemRect.y(), menuItemRect.width() - menuIconRect.width(), menuBarHeight), Qt::AlignCenter, menuText);
                    }
                    else
                    {
                        QRect menuIconRect(menuItemRect.x() + menuBarHeight * 0.1 + _menuBarItemMargin, menuBarHeight * 0.1, menuBarHeight * 0.8, menuBarHeight * 0.8);
                        painter->drawPixmap(menuIconRect, icon.pixmap(menuBarHeight * 0.8, menuBarHeight * 0.8));
                        painter->drawText(QRect(menuIconRect.right(), menuItemRect.y(), menuItemRect.width() - menuIconRect.width(), menuBarHeight), Qt::AlignCenter, menuText);
                    }
                }
            }
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

QSize ElaMenuBarStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case QStyle::CT_MenuBar:
    {
        QSize menuBarHeight = QProxyStyle::sizeFromContents(type, option, size, widget);
        menuBarHeight.setHeight(widget->height());
        return menuBarHeight;
    }
    case QStyle::CT_MenuBarItem:
    {
        if (const QStyleOptionMenuItem* mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option))
        {
            QSize menuBarItemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            menuBarItemSize.setHeight(widget->height());
            if ((!mopt->icon.isNull()) && !mopt->text.isEmpty())
            {
                menuBarItemSize.setWidth(menuBarItemSize.width() + mopt->fontMetrics.horizontalAdvance(mopt->text) + 2 * _menuBarItemMargin);
            }
            if (!mopt->text.contains("&"))
            {
                menuBarItemSize.setWidth(menuBarItemSize.width() + 5);
            }
            return menuBarItemSize;
        }
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

int ElaMenuBarStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_MenuBarHMargin:
    {
        return 0;
    }
    case QStyle::PM_MenuBarItemSpacing:
    {
        return 6;
    }
    case QStyle::PM_ToolBarExtensionExtent:
    {
        return 26;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}
