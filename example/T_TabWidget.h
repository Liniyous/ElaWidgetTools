#ifndef T_TABWIDGET_H
#define T_TABWIDGET_H

#include "ElaScrollPage.h"

class ElaTabWidget;
class T_TabWidget : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit T_TabWidget(QWidget* parent = nullptr);
    ~T_TabWidget();

private:
    ElaTabWidget* _tabWidget{nullptr};
};

#endif // T_TABWIDGET_H
