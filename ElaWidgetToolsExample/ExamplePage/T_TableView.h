#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_TABLEVIEW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_TABLEVIEW_H_

#include "T_BasePage.h"

class ElaTableView;
class T_TableView : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_TableView(QWidget* parent = nullptr);
    ~T_TableView();

private:
    ElaTableView* _tableView{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_TABLEVIEW_H_

