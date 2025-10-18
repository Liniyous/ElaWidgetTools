#ifndef ELACUSTOMTABWIDGET_H
#define ELACUSTOMTABWIDGET_H

#include "ElaCustomWidget.h"

class ElaTabBar;
class ElaTabWidget;
class ElaCustomTabWidget : public ElaCustomWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(bool, IsFinished)
public:
    explicit ElaCustomTabWidget(QWidget* parent = nullptr);
    ~ElaCustomTabWidget() override;
    void addTab(QWidget* widget, QIcon tabIcon, const QString& tabTitle);
    ElaTabBar* getCustomTabBar() const;
    ElaTabWidget* getCustomTabWidget() const;

private:
    ElaTabBar* _customTabBar{nullptr};
    ElaTabWidget* _customTabWidget{nullptr};
};

#endif // ELACUSTOMTABWIDGET_H
