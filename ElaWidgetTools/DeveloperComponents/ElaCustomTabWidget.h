#ifndef ELACUSTOMTABWIDGET_H
#define ELACUSTOMTABWIDGET_H

#include "ElaCustomWidget.h"

class ElaTabBar;
class ElaTabWidget;
class ElaCustomTabWidget : public ElaCustomWidget
{
    Q_OBJECT
public:
    explicit ElaCustomTabWidget(QWidget* parent = nullptr);
    ~ElaCustomTabWidget();
    void addTab(QWidget* widget, QIcon& tabIcon, const QString& tabTitle);
    ElaTabBar* getCustomTabBar() const;
    ElaTabWidget* getCustomTabWidget() const;

private:
    ElaTabBar* _customTabBar{nullptr};
    ElaTabWidget* _customTabWidget{nullptr};
};

#endif // ELACUSTOMTABWIDGET_H
