#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERHEADER_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERHEADER_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"

#include <QVBoxLayout>
#include <QWidget>

class QPropertyAnimation;
class ElaDrawerHeader : public QWidget
{
    Q_OBJECT
    ELA_VARIABLE(int, BorderRadius)
    ELA_VARIABLE(bool, IsExpand)
    ELA_PROPERTY(qreal, ExpandIconRotate)
public:
    explicit ElaDrawerHeader(QWidget* parent = nullptr);
    ~ElaDrawerHeader() override;

    void setHeaderWidget(QWidget* widget);
    void setIsBottomAttached(bool isAttached);
    bool getIsBottomAttached() const;
    void doExpandOrCollapseAnimation();

Q_SIGNALS:
    Q_SIGNAL void drawerHeaderClicked(bool isExpand);

protected:
    bool event(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    static bool _getIsHeaderClickable(const QWidget* posWidget);

    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light}; // 当前主题模式
    bool _isPressed{false};                                  // 鼠标左键是否按下标题可点击区域
    bool _isBottomAttached{false};                           // 底部是否与展开容器相连并保持直角
    QWidget* _headerWidget{nullptr};                         // 外部注入的自定义标题控件
    QVBoxLayout* _mainLayout{nullptr};                       // 标题内部主布局
    QPropertyAnimation* _rotateAnimation{nullptr};           // 右侧箭头旋转动画
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELADRAWERHEADER_H_
