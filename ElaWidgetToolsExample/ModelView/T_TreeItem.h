#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_TREEITEM_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_TREEITEM_H_

#include <QModelIndex>
#include <QObject>

#include "ElaPropertyMacro.h"
class T_TreeItem : public QObject
{
    Q_OBJECT
    ELA_PROPERTY(QList<T_TreeItem*>, ChildrenItems)
    ELA_PROPERTY(bool, IsChecked)
    ELA_VARIABLE(T_TreeItem*, ParentItem)
public:
    explicit T_TreeItem(const QString& itemTitle, T_TreeItem* parent = nullptr);
    ~T_TreeItem() override;
    QString getItemKey() const;
    QString getItemTitle() const;

    void setChildChecked(bool isChecked);
    Qt::CheckState getChildCheckState();

    void appendChildItem(T_TreeItem* childItem);
    bool getIsHasChild() const;
    int getRow() const;

private:
    QString _itemKey = "";
    QString _itemTitle = "";
    bool _isExpanded{false};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_TREEITEM_H_

