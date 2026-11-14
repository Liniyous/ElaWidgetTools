#include "ElaTableViewStyle.h"

#include "ElaColorAnimation.h"
#include "ElaControlStyle.h"
#include "ElaTableView.h"
#include "ElaTheme.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionViewItem>

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
        // 统一在有效索引处绘制选中 悬停与交替行背景 避免行级无效索引冲突
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            const ElaTableView* tabView = dynamic_cast<const ElaTableView*>(widget);
            if (!tabView)
            {
                return;
            }
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            QStyle::State itemState = vopt->state;
            if (tabView->selectionBehavior() == QAbstractItemView::SelectRows)
            {
                itemState.setFlag(QStyle::State_MouseOver, vopt->index.isValid() && vopt->index.row() == _pCurrentHoverRow);
            }
            const QColor background = ElaControlStyle::getItemBackgroundColor(_themeMode, itemState, false, vopt->features.testFlag(QStyleOptionViewItem::Alternate));
            painter->setPen(Qt::NoPen);
            painter->setBrush(ElaColorAnimation::getColor(widget, background, "CellBackground", vopt->index));
            painter->drawRect(vopt->rect);
            painter->restore();
        }
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
        painter->setPen(ElaThemeColor(_themeMode, BasicBorder));
        painter->setBrush(ElaColorAnimation::getColor(widget, ElaThemeColor(_themeMode, BasicBaseAlpha), "ViewBackground"));
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
        painter->setBrush(ElaColorAnimation::getColor(widget, ElaThemeColor(_themeMode, BasicBaseAlpha), "HeaderBackground"));
        painter->drawRect(option->rect);
        const auto* headerOption = qstyleoption_cast<const QStyleOptionHeader*>(option);
        const QByteArray role = "HeaderHover" + QByteArray::number(headerOption ? headerOption->section : -1);
        const QColor background = option->state.testFlag(QStyle::State_Sunken) ? ElaThemeColor(_themeMode, BasicPressAlpha)
            : option->state.testFlag(QStyle::State_MouseOver)                  ? ElaThemeColor(_themeMode, BasicHoverAlpha)
                                                                               : QColor(Qt::transparent);
        painter->setBrush(ElaColorAnimation::getColor(widget, background, role));
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
        painter->setBrush(ElaColorAnimation::getColor(widget, ElaThemeColor(_themeMode, BasicBaseAlpha), "HeaderEmptyBackground"));
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
                textRect = textRect.intersected(itemRect).adjusted(6, 0, -6, 0);
                QString text = vopt->text;
                text.replace('\n', ' ').replace('\r', ' ');
                painter->setFont(vopt->font);
                painter->drawText(textRect, vopt->displayAlignment | Qt::TextSingleLine, vopt->fontMetrics.elidedText(text, vopt->textElideMode, qMax(0, textRect.width())));
            }
            // 选中特效
            int heightOffset = itemRect.height() / 4;
            painter->setPen(Qt::NoPen);
            painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
            if (vopt->state.testFlag(QStyle::State_Selected))
            {
                if (selectionBehavior == QAbstractItemView::SelectRows && vopt->index.column() == 0)
                {
                    painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + heightOffset, 3, itemRect.height() - 2 * heightOffset), 1.5, 1.5);
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
