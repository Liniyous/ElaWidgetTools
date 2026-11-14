#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERCONTAINER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERCONTAINER_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"

#include <QList>
#include <QVBoxLayout>
#include <QWidget>

class QVariantAnimation;
class ElaDrawerContainer : public QWidget
{
    Q_OBJECT
    ELA_VARIABLE(int, BorderRadius)
public:
    explicit ElaDrawerContainer(QWidget* parent = nullptr);
    ~ElaDrawerContainer() override;

    void addWidget(QWidget* widget);
    void removeWidget(QWidget* widget);
    bool getHasWidgets() const;

    void doDrawerAnimation(bool isExpand);

Q_SIGNALS:
    Q_SIGNAL void containerCollapsed();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    int _getContentHeight() const;
    void _applyExpandStateImmediate(bool isExpand);

    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light}; // 当前主题模式
    bool _isExpanded{false};                                 // 目标展开状态
    bool _isAnimating{false};                                // 是否正在执行高度展开收回动画
    int _contentHeight{0};                                   // 动画期间缓存的内部内容完整高度
    QVBoxLayout* _mainLayout{nullptr};                       // 容器外层布局
    QVBoxLayout* _containerLayout{nullptr};                  // 抽屉项垂直排列布局
    QWidget* _containerWidget{nullptr};                      // 承载抽屉子项并参与滑动的内部面板
    QVariantAnimation* _heightAnimation{nullptr};            // 容器高度展开与收拢动画
    QList<QWidget*> _drawerWidgetList;                       // 已添加的抽屉子项列表
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERCONTAINER_H_
