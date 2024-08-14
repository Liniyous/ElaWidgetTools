#include "ElaListViewStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaListView.h"
#include "ElaTheme.h"
ElaListViewStyle::ElaListViewStyle(QStyle* style)
{
    _pItemHeight = 35;
    _pIsTransparent = false;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaListViewStyle::~ElaListViewStyle()
{
}

void ElaListViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
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
            itemRect.adjust(_margin, 2, -_margin, -2);
            QPainterPath path;
            path.addRoundedRect(itemRect, 4, 4);
            if (vopt->showDecorationSelected && (vopt->state & QStyle::State_Selected))
            {
                if (vopt->state & QStyle::State_MouseOver)
                {
                    // 选中时覆盖
                    painter->fillPath(path, ElaThemeColor(_themeMode, ListViewItemSelectedHover));
                }
                else
                {
                    // 选中
                    painter->fillPath(path, ElaThemeColor(_themeMode, ListViewItemSelected));
                }
            }
            else
            {
                if (vopt->state & QStyle::State_MouseOver)
                {
                    // 覆盖时颜色
                    painter->fillPath(path, ElaThemeColor(_themeMode, ListViewItemHover));
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

void ElaListViewStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        // viewport视口外的其他区域背景
        if (!_pIsTransparent)
        {
            QRect frameRect = option->rect;
            frameRect.adjust(1, 1, -1, -1);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(ElaThemeColor(_themeMode, ListViewBorder));
            painter->setBrush(ElaThemeColor(_themeMode, ListViewBase));
            painter->drawRoundedRect(frameRect, 3, 3);
            painter->restore();
        }
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
            const ElaListView* listView = dynamic_cast<const ElaListView*>(widget);
            QListView::ViewMode viewMode = listView->viewMode();
            // QRect checkRect = proxy()->subElementRect(SE_ItemViewItemCheckIndicator, vopt, widget);
            QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
            QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);
            iconRect.adjust(_leftPadding + _margin, 0, 0, 0);
            textRect.adjust(_leftPadding + _margin, 0, 0, 0);
            // 图标绘制
            if (!vopt->icon.isNull())
            {
                QIcon::Mode mode = QIcon::Normal;
                if (!(vopt->state.testFlag(QStyle::State_Enabled)))
                {
                    mode = QIcon::Disabled;
                }
                else if (vopt->state.testFlag(QStyle::State_Selected))
                {
                    mode = QIcon::Selected;
                }
                QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
                vopt->icon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
            }
            // 文字绘制
            if (!vopt->text.isEmpty())
            {
                painter->setPen(ElaThemeColor(_themeMode, WindowText));
                painter->drawText(textRect, vopt->displayAlignment, vopt->text);
            }
            // 选中特效
            if (vopt->state.testFlag(QStyle::State_Selected) && viewMode == QListView::ListMode)
            {
                int heightOffset = itemRect.height() / 4;
                painter->setPen(Qt::NoPen);
                painter->setBrush(ElaThemeColor(_themeMode, NavigationMark));
                painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + heightOffset, 3, itemRect.height() - 2 * heightOffset), 3, 3);
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

QSize ElaListViewStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case QStyle::CT_ItemViewItem:
    {
        QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        const ElaListView* listView = dynamic_cast<const ElaListView*>(widget);
        QListView::ViewMode viewMode = listView->viewMode();
        if (viewMode == QListView::ListMode)
        {
            itemSize.setWidth(itemSize.width() + _leftPadding + _margin * 2);
        }
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
