#include "ElaTableViewStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionViewItem>

#include "ElaTableView.h"
#include "ElaTheme.h"
ElaTableViewStyle::ElaTableViewStyle(QStyle* style)
{
    _pHeaderMargin = 6;
    _pCurrentHoverRow = -1;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaTableViewStyle::~ElaTableViewStyle()
{
}

void ElaTableViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    //qDebug() << element << option->rect << widget->objectName();
    switch (element)
    {
    case QStyle::PE_PanelItemViewItem:
    {
        // 行覆盖绘制
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            const ElaTableView* tabView = dynamic_cast<const ElaTableView*>(widget);
            if (!tabView)
            {
                return;
            }
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            QAbstractItemView::SelectionBehavior selectionBehavior = tabView->selectionBehavior();
            if (selectionBehavior == QAbstractItemView::SelectRows)
            {
                if (vopt->index.row() == _pCurrentHoverRow)
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
                    painter->drawRect(vopt->rect);
                }
            }
            else
            {
                if (vopt->state.testFlag(QStyle::State_Selected) || vopt->state.testFlag(QStyle::State_MouseOver))
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
                    painter->drawRect(vopt->rect);
                }
            }
            painter->restore();
        }
        return;
    }
    case QStyle::PE_PanelItemViewRow:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            QRect itemRect = vopt->rect;
            painter->setPen(Qt::NoPen);
            if (vopt->state & QStyle::State_Selected)
            {
                // 选中
                painter->setBrush(ElaThemeColor(_themeMode, BasicSelectedAlpha));
                painter->drawRect(itemRect);
            }
            else
            {
                if (vopt->features.testFlag(QStyleOptionViewItem::Alternate))
                {
                    // Item背景隔行变色
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(ElaThemeColor(_themeMode, BasicAlternating));
                    painter->drawRect(vopt->rect);
                }
            }
            painter->restore();
        }
        return;
    }
    case QStyle::PE_Widget:
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

void ElaTableViewStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    //qDebug() << element << option->rect;
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
        painter->setBrush(ElaThemeColor(_themeMode, BasicBaseDeep));
        painter->drawRect(option->rect);
        if (option->state.testFlag(QStyle::State_Sunken))
        {
            painter->setBrush(ElaThemeColor(_themeMode, BasicPressAlpha));
        }
        else
        {
            if (option->state.testFlag(QStyle::State_MouseOver))
            {
                painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
            }
        }
        painter->drawRect(option->rect);
        painter->restore();
        return;
    }
    case QStyle::CE_HeaderEmptyArea:
    {
        // 表头未使用区域背景绘制
        QRect frameRect = option->rect;
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(ElaThemeColor(_themeMode, BasicBaseDeep));
        painter->drawRect(frameRect);
        painter->restore();
        return;
    }
    case QStyle::CE_ItemViewItem:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            // 背景绘制
            const ElaTableView* tabView = dynamic_cast<const ElaTableView*>(widget);
            if (!tabView)
            {
                return;
            }
            this->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);
            QAbstractItemView::SelectionBehavior selectionBehavior = tabView->selectionBehavior();
            // 内容绘制
            QRect itemRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
            // QRect checkRect = proxy()->subElementRect(SE_ItemViewItemCheckIndicator, vopt, widget);
            QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
            QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);
            if (vopt->index.column() == 0)
            {
                iconRect.adjust(_horizontalPadding, 0, _horizontalPadding, 0);
                textRect.adjust(_horizontalPadding, 0, 0, 0);
            }
            // 图标绘制
            if (!vopt->icon.isNull())
            {
                QIcon::Mode mode = QIcon::Normal;
                // if (!(vopt->state.testFlag(QStyle::State_Enabled)))
                // {
                //     mode = QIcon::Disabled;
                // }
                // else if (vopt->state.testFlag(QStyle::State_Selected))
                // {
                //     mode = QIcon::Selected;
                // }
                QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
                vopt->icon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
            }
            // 文字绘制
            if (!vopt->text.isEmpty())
            {
                painter->setPen(ElaThemeColor(_themeMode, BasicText));
                painter->drawText(textRect, vopt->displayAlignment, vopt->text);
            }
            // 选中特效
            int heightOffset = itemRect.height() / 4;
            painter->setPen(Qt::NoPen);
            painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
            if (vopt->state.testFlag(QStyle::State_Selected))
            {
                if (selectionBehavior == QAbstractItemView::SelectRows && vopt->index.column() == 0)
                {
                    painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + heightOffset, 3, itemRect.height() - 2 * heightOffset), 3, 3);
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

int ElaTableViewStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    //qDebug() << metric << QProxyStyle::pixelMetric(metric, option, widget);
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
