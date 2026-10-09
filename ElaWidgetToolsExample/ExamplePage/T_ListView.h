#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_LISTVIEW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_LISTVIEW_H_

#include "T_BasePage.h"

class ElaListView;
class T_ListView : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_ListView(QWidget* parent = nullptr);
    ~T_ListView();

private:
    ElaListView* _listView{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_LISTVIEW_H_

