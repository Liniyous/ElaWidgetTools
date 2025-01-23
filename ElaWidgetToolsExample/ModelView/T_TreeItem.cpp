#include "T_TreeItem.h"

#include <QUuid>
T_TreeItem::T_TreeItem(QString itemTitle, T_TreeItem* parent)
    : QObject{parent}
{
    _itemKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    _itemTitle = itemTitle;
    _pParentItem = parent;
    _pIsChecked = false;
}

T_TreeItem::~T_TreeItem()
{
    qDeleteAll(_pChildrenItems);
}

QString T_TreeItem::getItemKey() const
{
    return _itemKey;
}

QString T_TreeItem::getItemTitle() const
{
    return _itemTitle;
}

void T_TreeItem::setChildChecked(bool isChecked)
{
    if (isChecked)
    {
        for (auto node : _pChildrenItems)
        {
            node->setIsChecked(isChecked);
            node->setChildChecked(isChecked);
        }
    }
    else
    {
        for (auto node : _pChildrenItems)
        {
            node->setChildChecked(isChecked);
            node->setIsChecked(isChecked);
        }
    }
}

Qt::CheckState T_TreeItem::getChildCheckState()
{
    bool isAllChecked = true;
    bool isAnyChecked = false;
    for (auto node : _pChildrenItems)
    {
        if (node->getIsChecked())
        {
            isAnyChecked = true;
        }
        else
        {
            isAllChecked = false;
        }
        Qt::CheckState childState = node->getChildCheckState();
        if (childState == Qt::PartiallyChecked)
        {
            isAllChecked = false;
            isAnyChecked = true;
            break;
        }
        else if (childState == Qt::Unchecked)
        {
            isAllChecked = false;
        }
    }
    if (_pChildrenItems.count() > 0)
    {
        if (isAllChecked)
        {
            return Qt::Checked;
        }
        if (isAnyChecked)
        {
            return Qt::PartiallyChecked;
        }
        return Qt::Unchecked;
    }
    return Qt::Checked;
}

void T_TreeItem::appendChildItem(T_TreeItem* childItem)
{
    _pChildrenItems.append(childItem);
}

bool T_TreeItem::getIsHasChild() const
{
    if (_pChildrenItems.count() > 0)
    {
        return true;
    }
    return false;
}

int T_TreeItem::getRow() const
{
    if (_pParentItem)
    {
        return _pParentItem->getChildrenItems().indexOf(const_cast<T_TreeItem*>(this));
    }
    return 0;
}
