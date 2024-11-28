#ifndef T_NAVIGATION_H
#define T_NAVIGATION_H

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

#endif // T_NAVIGATION_H
