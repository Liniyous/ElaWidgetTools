#ifndef T_VIEW_H
#define T_VIEW_H

#include "T_BasePage.h"

class ElaListView;
class ElaTableView;
class ElaTreeView;
class T_View : public T_BasePage
{
public:
    explicit T_View(QWidget* parent = nullptr);
    ~T_View();

private:
    ElaListView* _listView{nullptr};
    ElaTableView* _tableView{nullptr};
    ElaTreeView* _treeView{nullptr};
};

#endif // T_VIEW_H
