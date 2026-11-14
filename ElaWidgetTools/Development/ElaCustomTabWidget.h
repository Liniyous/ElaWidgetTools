#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELACUSTOMTABWIDGET_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELACUSTOMTABWIDGET_H_

#include "ElaCustomWidget.h"

class ElaTabWidget;

// 可归还页面的临时标签浮窗
class ElaCustomTabWidget : public ElaCustomWidget
{
    Q_OBJECT
public:
    explicit ElaCustomTabWidget(QWidget* parent = nullptr);
    ElaTabWidget* getCustomTabWidget() const;
    void reject() override;

protected:
    void closeEvent(QCloseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    Q_INVOKABLE bool _getIsTitleBarDraggable() const;

    ElaTabWidget* _customTabWidget{nullptr}; // 与主窗口共用的标签容器
    QWidget* _tabBarWidget{nullptr};         // 标题栏中的固定和普通标签行
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELACUSTOMTABWIDGET_H_
