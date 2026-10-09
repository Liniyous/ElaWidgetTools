#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACUSTOMTABWIDGET_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACUSTOMTABWIDGET_H_

#include "ElaCustomWidget.h"
#include "ElaPropertyMacro.h"

class ElaTabBar;
class ElaTabWidget;
class ElaCustomTabWidget : public ElaCustomWidget
{
    Q_OBJECT
    ELA_PROPERTY(bool, IsFinished)
public:
    explicit ElaCustomTabWidget(QWidget* parent = nullptr);
    ~ElaCustomTabWidget() override;
    void addTab(QWidget* widget, QIcon tabIcon, const QString& tabTitle);
    ElaTabBar* getCustomTabBar() const;
    ElaTabWidget* getCustomTabWidget() const;

    Q_INVOKABLE bool processHitTest();

private:
    bool _isAllowLeave{false};
    ElaTabBar* _customTabBar{nullptr};
    ElaTabWidget* _customTabWidget{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACUSTOMTABWIDGET_H_

