#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_TREEVIEW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_TREEVIEW_H_

#include "T_BasePage.h"

class ElaTreeView;
class T_TreeView : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_TreeView(QWidget* parent = nullptr);
    ~T_TreeView();

private:
    ElaTreeView* _treeView{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_TREEVIEW_H_

