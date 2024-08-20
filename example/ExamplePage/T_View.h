#ifndef T_VIEW_H
#define T_VIEW_H

#include <ElaScrollPage.h>

class ElaListView;
class ElaTableView;
class T_View : public ElaScrollPage
{
public:
    explicit T_View(QWidget* parent = nullptr);
    ~T_View();

private:
    ElaListView* _listView{nullptr};
    ElaTableView* _tableView{nullptr};
};

#endif // T_VIEW_H
