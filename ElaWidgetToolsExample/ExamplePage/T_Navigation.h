#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_NAVIGATION_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_NAVIGATION_H_

#include "T_BasePage.h"

class ElaPivot;
class ElaTabWidget;
class ElaBreadcrumbBar;
class T_Navigation : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Navigation(QWidget* parent = nullptr);
    ~T_Navigation();

private:
    ElaBreadcrumbBar* _breadcrumbBar{nullptr};
    ElaPivot* _pivot{nullptr};
    ElaTabWidget* _tabWidget{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_EXAMPLEPAGE_T_NAVIGATION_H_

