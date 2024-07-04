#include "ElaMenuStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaApplication.h"
#include "ElaMenu.h"
ElaMenuStyle::ElaMenuStyle(QStyle* style)
{
    _windowLinearGradient = new QLinearGradient(0, 0, 100, 100);
    _windowLinearGradient->setColorAt(0, QColor(0xF2, 0xE7, 0xF5));
    _windowLinearGradient->setColorAt(1, QColor(0xED, 0xF3, 0xF9));
    _themeMode = ElaApplication::getInstance()->getThemeMode();
    onThemeChanged(ElaApplication::getInstance()->getThemeMode());
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, &ElaMenuStyle::onThemeChanged);
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
        painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        QColor color = QColor(0x60, 0x5F, 0x61);
        for (int i = 0; i < _shadowBorderWidth; i++)
        {
            QPainterPath path;
            path.setFillRule(Qt::WindingFill);
            path.addRoundedRect(_shadowBorderWidth - i, _shadowBorderWidth - i, option->rect.width() - (_shadowBorderWidth - i) * 2, option->rect.height() - (_shadowBorderWidth - i) * 2, 6 + i, 6 + i);
            int alpha = 5 * (_shadowBorderWidth - i + 1);
            color.setAlpha(alpha > 255 ? 255 : alpha);
            painter->setPen(color);
            painter->drawPath(path);
        }
        //内容色
        QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth, option->rect.width() - 2 * _shadowBorderWidth, option->rect.height() - 2 * _shadowBorderWidth);
        painter->setPen(Qt::NoPen);
        _windowLinearGradient->setFinalStop(foregroundRect.bottomRight());
        painter->setBrush(*_windowLinearGradient);
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
                painter->setBrush(QColor(0xB3, 0xB3, 0xB3));
                painter->drawRoundedRect(QRectF(separatorRect.x() + separatorRect.width() * 0.05, separatorRect.y(), separatorRect.width() - separatorRect.width() * 0.1, 1.5), 1, 1);
                painter->restore();
                return;
            }
            else
            {
                QRect menuRect = mopt->rect;
                painter->save();
                painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
                //覆盖效果
                if (mopt->state.testFlag(QStyle::State_MouseOver) || mopt->state.testFlag(QStyle::State_Selected))
                {
                    QRect hoverRect = menuRect;
                    hoverRect.setTop(hoverRect.top() + 2);
                    hoverRect.setLeft(hoverRect.left() + 4);
                    hoverRect.setRight(hoverRect.right() - 4);
                    hoverRect.setBottom(hoverRect.bottom() - 2);
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(_themeMode == ElaApplicationType::Light ? QColor(0xE5, 0xE3, 0xE6) : QColor(0x46, 0x46, 0x46));
                    painter->drawRoundedRect(hoverRect, 5, 5);
                }
                //Icon绘制
                QIcon menuIcon = mopt->icon;
                if (!menuIcon.isNull())
                {
                    painter->drawPixmap(QRect(menuRect.x() + _iconLeftPadding, menuRect.center().y() - _iconWidth / 2, _iconWidth, _iconWidth), menuIcon.pixmap(_iconWidth, _iconWidth));
                }
                else
                {
                    const ElaMenu* menu = dynamic_cast<const ElaMenu*>(widget);
                    if (menu)
                    {
                        QAction* action = menu->actionAt(menuRect.center());
                        if (action)
                        {
                            QString iconText = action->property("ElaIconType").toString();
                            if (!iconText.isEmpty())
                            {
                                painter->save();
                                painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
                                QFont iconFont = QFont("ElaAwesome");
                                iconFont.setPixelSize(20);
                                painter->setFont(iconFont);
                                painter->drawText(QRect(menuRect.x() + _iconLeftPadding, menuRect.center().y() - _iconWidth / 2, _iconWidth, _iconWidth), iconText);
                                painter->restore();
                            }
                        }
                    }
                }
                //文字和快捷键绘制
                //QString iconText = QChar((unsigned short)icon) + QString("#ElaIconType#") + text;
                if (!mopt->text.isEmpty())
                {
                    QStringList textList = mopt->text.split("\t");
                    painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
                    painter->drawText(QRect(menuRect.x() + _iconLeftPadding + _iconWidth + _textLeftSpacing, menuRect.y(), menuRect.width(), menuRect.height()), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, textList[0]);
                    if (textList.count() > 1)
                    {
                        painter->drawText(QRect(menuRect.x() + _iconLeftPadding + _iconWidth + _textLeftSpacing, menuRect.y(), menuRect.width() - (_iconLeftPadding + _iconWidth + _textLeftSpacing + 15), menuRect.height()), Qt::AlignRight | Qt::AlignVCenter | Qt::TextSingleLine, textList[1]);
                    }
                }
                //展开图标
                if (mopt->menuItemType == QStyleOptionMenuItem::SubMenu)
                {
                    painter->save();
                    painter->setPen(_themeMode == ElaApplicationType::Light ? Qt::black : Qt::white);
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(18);
                    painter->setFont(iconFont);
                    painter->drawText(QRect(menuRect.right() - 25, menuRect.y(), 25, menuRect.height()), Qt::AlignVCenter, QChar((unsigned short)ElaIconType::AngleRight));
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
        return _iconWidth;
    }
    case QStyle::PM_MenuPanelWidth:
    {
        //外围容器宽度
        return _shadowBorderWidth * 1.5;
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
            return QSize(menuItemSize.width() + 20, _menuItemHeight);
        }
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void ElaMenuStyle::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    _themeMode = themeMode;
    if (themeMode == ElaApplicationType::Light)
    {
        _windowLinearGradient->setColorAt(0, QColor(0xF2, 0xE7, 0xF5));
        _windowLinearGradient->setColorAt(1, QColor(0xED, 0xF3, 0xF9));
    }
    else
    {
        _windowLinearGradient->setColorAt(0, QColor(0x2D, 0x2D, 0x2D));
        _windowLinearGradient->setColorAt(1, QColor(0x3D, 0x3D, 0x3D));
    }
}
