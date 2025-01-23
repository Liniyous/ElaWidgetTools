#include "ElaColorDisplayDelegate.h"

#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "ElaTheme.h"
ElaColorDisplayDelegate::ElaColorDisplayDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _pThemeMode = eTheme->getThemeMode();
}

ElaColorDisplayDelegate::~ElaColorDisplayDelegate()
{
}

void ElaColorDisplayDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    QRectF itemRect = option.rect;
    // 颜色球绘制
    QColor displayColor = index.data(Qt::UserRole).value<QColor>();
    if (displayColor.isValid())
    {
        painter->setPen(ElaThemeColor(_pThemeMode, PopupBorderHover));
        painter->setBrush(index.data(Qt::UserRole).value<QColor>());
    }
    else
    {
        QPen pen(ElaThemeColor(_pThemeMode, PopupBorderHover));
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
    }
    painter->drawEllipse(itemRect.center(), 10, 10);

    // 覆盖效果绘制
    if (option.state.testFlag(QStyle::State_MouseOver) || option.state.testFlag(QStyle::State_Selected))
    {
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(itemRect.center(), 13, 13);
    }

    painter->restore();
    QStyledItemDelegate::paint(painter, option, index);
}

QSize ElaColorDisplayDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(30, 30);
}
