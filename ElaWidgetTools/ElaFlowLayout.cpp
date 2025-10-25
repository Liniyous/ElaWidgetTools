#include "ElaFlowLayout.h"

#include <QPropertyAnimation>
#include <QWidget>

#include "private/ElaFlowLayoutPrivate.h"

ElaFlowLayout::ElaFlowLayout(QWidget* parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), d_ptr(new ElaFlowLayoutPrivate())
{
    Q_D(ElaFlowLayout);
    d->q_ptr = this;
    d->_hSpacing = hSpacing;
    d->_vSpacing = vSpacing;
    setContentsMargins(margin, margin, margin, margin);
}

ElaFlowLayout::ElaFlowLayout(int margin, int hSpacing, int vSpacing)
    : d_ptr(new ElaFlowLayoutPrivate())
{
    Q_D(ElaFlowLayout);
    d->q_ptr = this;
    d->_hSpacing = hSpacing;
    d->_vSpacing = vSpacing;
    setContentsMargins(margin, margin, margin, margin);
}

ElaFlowLayout::~ElaFlowLayout()
{
    QLayoutItem* item;
    while ((item = ElaFlowLayout::takeAt(0)))
    {
        delete item;
    }
}

void ElaFlowLayout::addItem(QLayoutItem* item)
{
    Q_D(ElaFlowLayout);
    d->_itemList.append(item);
}

int ElaFlowLayout::horizontalSpacing() const
{
    Q_D(const ElaFlowLayout);
    if (d->_hSpacing >= 0)
    {
        return d->_hSpacing;
    }
    else
    {
        return d->_smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int ElaFlowLayout::verticalSpacing() const
{
    Q_D(const ElaFlowLayout);
    if (d->_vSpacing >= 0)
    {
        return d->_vSpacing;
    }
    else
    {
        return d->_smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}
int ElaFlowLayout::count() const
{
    Q_D(const ElaFlowLayout);
    return d->_itemList.size();
}

QLayoutItem* ElaFlowLayout::itemAt(int index) const
{
    Q_D(const ElaFlowLayout);
    return d->_itemList.value(index);
}

QLayoutItem* ElaFlowLayout::takeAt(int index)
{
    Q_D(ElaFlowLayout);
    if (index >= 0 && index < d->_itemList.size())
    {
        return d->_itemList.takeAt(index);
    }
    return nullptr;
}

void ElaFlowLayout::setIsAnimation(bool isAnimation)
{
    Q_D(ElaFlowLayout);
    d->_isAnimation = isAnimation;
}

Qt::Orientations ElaFlowLayout::expandingDirections() const
{
    return {};
}

bool ElaFlowLayout::hasHeightForWidth() const
{
    return true;
}

int ElaFlowLayout::heightForWidth(int width) const
{
    Q_D(const ElaFlowLayout);
    return d->_doLayout(QRect(0, 0, width, 0), true);
}

void ElaFlowLayout::setGeometry(const QRect& rect)
{
    Q_D(ElaFlowLayout);
    QLayout::setGeometry(rect);
    d->_doLayout(rect, false);
}

QSize ElaFlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize ElaFlowLayout::minimumSize() const
{
    Q_D(const ElaFlowLayout);
    QSize size;
    for (const QLayoutItem* item: d->_itemList)
    {
        size = size.expandedTo(item->minimumSize());
    }
    const QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}
