#include "ElaColorDisplayDelegate.h"

#include "ElaTheme.h"

#include <QColor>
#include <QPainter>
#include <QStyleOption>

ElaColorDisplayDelegate::ElaColorDisplayDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
}

ElaColorDisplayDelegate::~ElaColorDisplayDelegate()
{
}

void ElaColorDisplayDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    QRectF itemRect = option.rect;
    const auto themeMode = eTheme->getThemeMode();
    // 颜色球绘制
    const QColor displayColor = index.data(Qt::UserRole).value<QColor>();
    QPen outlinePen(ElaThemeColor(themeMode, BasicBorderDeep));
    painter->setBrush(Qt::NoBrush);
    if (displayColor.isValid())
    {
        painter->setBrush(displayColor);
    }
    else
    {
        outlinePen.setStyle(Qt::DashLine);
    }
    painter->setPen(outlinePen);
    painter->drawEllipse(itemRect.center(), 10, 10);

    // 覆盖效果绘制
    if (option.state.testFlag(QStyle::State_MouseOver) || option.state.testFlag(QStyle::State_Selected))
    {
        painter->setBrush(Qt::NoBrush);
        const bool isSelected = option.state.testFlag(QStyle::State_Selected);
        painter->setPen(QPen(isSelected ? ElaThemeColor(themeMode, PrimaryNormal) : ElaThemeColor(themeMode, BasicTextNoFocus), isSelected ? 2 : 1));
        painter->drawEllipse(itemRect.center(), 13, 13);
    }

    painter->restore();
}

QSize ElaColorDisplayDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(30, 30);
}
