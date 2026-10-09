#include "ElaActionCommanderViewStyle.h"

#include "ElaActionCommanderNode.h"
#include "ElaApplication.h"
#include "ElaWidgetToolsDef.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaTheme.h"
ElaActionCommanderViewStyle::ElaActionCommanderViewStyle(QStyle* style)
{
    _pItemHeight = 40;
    _pHeaderMargin = 5;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaActionCommanderViewStyle::~ElaActionCommanderViewStyle()
{
}

void ElaActionCommanderViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelItemViewItem:
    {
        // Item背景
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            QRect itemRect = vopt->rect;
            itemRect.adjust(0, 2, 0, -2);
            painter->setPen(Qt::NoPen);
            bool isEnable = vopt->state.testFlag(QStyle::State_Enabled);
            if (vopt->state & QStyle::State_Selected)
            {
                if (vopt->state & QStyle::State_MouseOver && isEnable)
                {
                    // 选中时覆盖
                    painter->setBrush(ElaThemeColor(_themeMode, BasicSelectedHoverAlpha));
                }
                else
                {
                    // 选中
                    painter->setBrush(ElaThemeColor(_themeMode, BasicSelectedAlpha));
                }
            }
            else
            {
                if (vopt->state & QStyle::State_MouseOver && isEnable)
                {
                    // 覆盖时颜色
                    painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
                }
            }
            painter->drawRoundedRect(itemRect, 4, 4);
            painter->restore();
        }
        return;
    }
    case QStyle::PE_IndicatorBranch:
    {
        // 展开图标绘制
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            if (vopt->state.testFlag(QStyle::State_Children))
            {
                painter->save();
                QRect indicatorRect = option->rect;
                indicatorRect.adjust(0, 0, -2, 0);
                QFont iconFont = QFont("ElaAwesome");
                iconFont.setPixelSize(eApp->getFontPixelSize() + 4);
                painter->setFont(iconFont);
                painter->setPen(ElaThemeColor(_themeMode, BasicText));
                painter->drawText(indicatorRect, Qt::AlignVCenter | Qt::AlignRight, vopt->state.testFlag(QStyle::State_Open) ? QChar(ElaIconType::AngleDown) : QChar(ElaIconType::AngleRight));
                painter->restore();
            }
        }
        return;
    }
    case QStyle::PE_IndicatorItemViewItemDrop:
    {
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter->setPen(ElaThemeColor(_themeMode, BasicText));
        painter->setBrush(Qt::NoBrush);
        QRect optionRect = option->rect;
        if (optionRect.height() == 0)
        {
            painter->drawLine(optionRect.topLeft(), optionRect.topRight());
        }
        else
        {
            painter->drawRect(optionRect);
        }
        painter->restore();
        return;
    }
    case QStyle::PE_PanelItemViewRow:
    {
        return;
    }
    case QStyle::PE_Widget:
    {
        return;
    }
    case QStyle::PE_Frame:
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

void ElaActionCommanderViewStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    int fontPixelSize = eApp->getFontPixelSize();
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        // viewport视口外的其他区域背景
        QRect frameRect = option->rect;
        frameRect.adjust(1, 1, -1, -1);
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        painter->setPen(ElaThemeColor(_themeMode, PopupBorder));
        painter->setBrush(ElaThemeColor(_themeMode, BasicBaseAlpha));
        painter->drawRoundedRect(frameRect, 3, 3);
        painter->restore();
        return;
    }
    case QStyle::CE_HeaderLabel:
    {
        // 表头文字绘制
        if (const QStyleOptionHeader* hopt = qstyleoption_cast<const QStyleOptionHeader*>(option))
        {
            QRect headerRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
            if (!hopt->text.isEmpty())
            {
                painter->setPen(ElaThemeColor(_themeMode, BasicText));
                painter->drawText(headerRect, hopt->textAlignment, hopt->text);
            }
            painter->restore();
        }
        return;
    }
    case QStyle::CE_HeaderSection:
    {
        // 表头背景绘制
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(ElaThemeColor(_themeMode, BasicBaseDeepAlpha));
        painter->drawRect(option->rect);
        painter->restore();
        return;
    }
    case QStyle::CE_ItemViewItem:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            // 背景绘制
            this->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);
            // 内容绘制
            QRect itemRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
            QRect checkRect = proxy()->subElementRect(SE_ItemViewItemCheckIndicator, vopt, widget);
            QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
            QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);
            // 复选框绘制
            if (checkRect.isValid())
            {
                painter->save();
                //图标绘制
                if (vopt->checkState == Qt::Checked)
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
                    painter->drawRoundedRect(checkRect, 2, 2);
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(checkRect.width() * 0.85);
                    painter->setFont(iconFont);
                    painter->setPen(ElaThemeColor(ElaThemeType::Dark, BasicText));
                    painter->drawText(checkRect, Qt::AlignCenter, QChar(ElaIconType::Check));
                }
                else if (vopt->checkState == Qt::PartiallyChecked)
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
                    painter->drawRoundedRect(checkRect, 2, 2);
                    painter->setPen(ElaThemeColor(ElaThemeType::Dark, BasicText));
                    QLine checkLine(checkRect.x() + 3, checkRect.center().y(), checkRect.right() - 3, checkRect.center().y());
                    painter->drawLine(checkLine);
                }
                else
                {
                    painter->setPen(QPen(ElaThemeColor(_themeMode, BasicBorderDeep), 1.2));
                    painter->setBrush(Qt::transparent);
                    painter->drawRoundedRect(checkRect, 2, 2);
                }
                painter->restore();
            }
            // 图标绘制
            if (!vopt->icon.isNull())
            {
                QIcon::Mode mode = QIcon::Normal;
                QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
                vopt->icon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
            }
            // 消息类型绘制
            auto modelIndex = vopt->index;
            const auto indexNode = static_cast<ElaActionCommanderNode*>(modelIndex.internalPointer());
            if (indexNode && indexNode->getIsCommandNode())
            {
                painter->save();
                switch (indexNode->getMessageMode())
                {
                case ElaMessageBarType::Success:
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(QColor(0x01, 0xE5, 0x7B));
                    painter->drawEllipse(QPoint(textRect.x(), textRect.y() + textRect.height() / 2), 10, 10);
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(fontPixelSize - 1);
                    painter->setFont(iconFont);
                    painter->setPen(Qt::black);
                    painter->drawText(textRect.x() - 5, textRect.y(), textRect.width(), textRect.height(), Qt::AlignVCenter | Qt::AlignLeft, QChar(ElaIconType::Check));
                    break;
                }
                case ElaMessageBarType::Information:
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(QColor(0xF4, 0xF4, 0xF4));
                    painter->drawEllipse(QPoint(textRect.x(), textRect.y() + textRect.height() / 2), 10, 10);
                    QFont iconFont = painter->font();
                    iconFont.setPixelSize(fontPixelSize);
                    iconFont.setBold(true);
                    painter->setFont(iconFont);
                    painter->setPen(Qt::black);
                    painter->drawText(textRect.x() - 2, textRect.y(), textRect.width(), textRect.height(), Qt::AlignVCenter | Qt::AlignLeft, "i");
                    break;
                }
                case ElaMessageBarType::Warning:
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(QColor(0xFA, 0xB6, 0x00));
                    painter->drawEllipse(QPoint(textRect.x(), textRect.y() + textRect.height() / 2), 10, 10);
                    QFont iconFont = painter->font();
                    iconFont.setPixelSize(fontPixelSize);
                    iconFont.setBold(true);
                    painter->setFont(iconFont);
                    painter->setPen(Qt::black);
                    painter->drawText(textRect.x() - 3, textRect.y(), textRect.width(), textRect.height(), Qt::AlignVCenter | Qt::AlignLeft, "!");
                    break;
                }
                case ElaMessageBarType::Error:
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(ElaThemeColor(_themeMode, StatusDanger));
                    painter->drawEllipse(QPoint(textRect.x(), textRect.y() + textRect.height() / 2), 10, 10);
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(fontPixelSize);
                    painter->setFont(iconFont);
                    painter->setPen(Qt::white);
                    painter->drawText(textRect.x() - 5, textRect.y(), textRect.width(), textRect.height(), Qt::AlignVCenter | Qt::AlignLeft, QChar(ElaIconType::Xmark));
                    break;
                }
                default:
                {
                    break;
                }
                }
                painter->restore();
            }
            // 文字绘制
            bool isEnable = vopt->state.testFlag(QStyle::State_Enabled);
            if (!vopt->text.isEmpty())
            {
                painter->setPen(isEnable ? ElaThemeColor(_themeMode, BasicText) : ElaThemeColor(_themeMode, BasicTextDisable));
                painter->drawText(textRect, vopt->displayAlignment, vopt->text);
            }
            // 选中特效
            int heightOffset = itemRect.height() / 4;
            painter->setPen(Qt::NoPen);
            painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
            if (vopt->state.testFlag(QStyle::State_Selected) && (vopt->viewItemPosition == QStyleOptionViewItem::Beginning || vopt->viewItemPosition == QStyleOptionViewItem::OnlyOne))
            {
                painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + heightOffset, 3, itemRect.height() - 2 * heightOffset), 1.5, 1.5);
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

QSize ElaActionCommanderViewStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case QStyle::CT_ItemViewItem:
    {
        QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        itemSize.setHeight(_pItemHeight);
        return itemSize;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

int ElaActionCommanderViewStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_HeaderMargin:
    {
        return _pHeaderMargin;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

QRect ElaActionCommanderViewStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::SE_ItemViewItemCheckIndicator:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            if (vopt->viewItemPosition != QStyleOptionViewItem::Middle && vopt->viewItemPosition != QStyleOptionViewItem::End)
            {
                QRect indicatorRect = QProxyStyle::subElementRect(element, option, widget);
                indicatorRect.adjust(_leftPadding, 0, _leftPadding, 0);
                return indicatorRect;
            }
        }
        break;
    }
    case QStyle::SE_ItemViewItemDecoration:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            if (vopt->viewItemPosition != QStyleOptionViewItem::Middle && vopt->viewItemPosition != QStyleOptionViewItem::End)
            {
                QRect iconRect = QProxyStyle::subElementRect(element, option, widget);
                iconRect.adjust(_leftPadding + 5, 0, _leftPadding + 5, 0);
                return iconRect;
            }
        }
        break;
    }
    case QStyle::SE_ItemViewItemText:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            if (vopt->viewItemPosition != QStyleOptionViewItem::Middle && vopt->viewItemPosition != QStyleOptionViewItem::End)
            {
                QRect textRect = QProxyStyle::subElementRect(element, option, widget);
                textRect.adjust(_leftPadding + 10, 0, 0, 0);
                return textRect;
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::subElementRect(element, option, widget);
}

ElaThemeType::ThemeMode ElaActionCommanderViewStyle::getThemeMode() const
{
    return _themeMode;
}
