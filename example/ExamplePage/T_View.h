#ifndef T_VIEW_H
#define T_VIEW_H

#include <ElaScrollPage.h>

class ElaListView;
class T_View : public ElaScrollPage
{
public:
    explicit T_View(QWidget* parent = nullptr);
    ~T_View();

private:
    ElaListView* _listView{nullptr};
};

#endif // T_VIEW_H
