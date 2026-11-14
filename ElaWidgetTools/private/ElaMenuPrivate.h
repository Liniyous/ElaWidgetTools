#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMENUPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMENUPRIVATE_H_

#include <QList>
#include <QObject>
#include <QPixmap>
#include <QPointer>

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsExport.h"
class ElaMenu;
class ElaMenuStyle;
class QPropertyAnimation;
class QWidget;
class ElaMenuPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaMenu)
    ELA_PROPERTY(qreal, AnimationImagePosY)
public:
    explicit ElaMenuPrivate(QObject* parent = nullptr);
    ~ElaMenuPrivate() override;

private:
    QPixmap _animationPix;                       // 菜单及嵌入控件的滑动截图
    QList<QPointer<QWidget>> _animationWidgets;  // 截图播放期间暂时隐藏的控件
    QPointer<QWidget> _animationFocusWidget;     // 动画结束后恢复输入焦点的控件
    QPropertyAnimation* _showAnimation{nullptr}; // 菜单展开时的截图位移动画
    ElaMenuStyle* _menuStyle{nullptr};           // 菜单主题样式

    QWidget* _getPopupSource() const;
    void _startShowAnimation(bool isSlideDown, int bodyHeight);
    void _finishShowAnimation();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMENUPRIVATE_H_
