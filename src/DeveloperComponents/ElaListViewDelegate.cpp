#include "ElaListViewDelegate.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
ElaListViewDelegate::ElaListViewDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaListViewDelegate::~ElaListViewDelegate()
{
}

void ElaListViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    QRect itemRect = option.rect;
    itemRect.setTop(itemRect.top() + 2);
    itemRect.setBottom(itemRect.bottom() - 2);
    QPainterPath path;
    path.addRoundedRect(itemRect, 8, 8);
    if (option.showDecorationSelected && (option.state & QStyle::State_Selected))
    {
        if (option.state & QStyle::State_MouseOver)
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
        if (option.state & QStyle::State_MouseOver)
        {
            // 覆盖时颜色
            painter->fillPath(path, ElaThemeColor(_themeMode, ListViewItemHover));
        }
    }
    //文字绘制
    QString text = index.data(Qt::DisplayRole).toString();
    painter->setPen(ElaThemeColor(_themeMode, WindowText));
    painter->drawText(itemRect, Qt::AlignVCenter | Qt::AlignLeft, text);
    painter->restore();
}
