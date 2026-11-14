#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELACOMBOBOXPOPUP_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELACOMBOBOXPOPUP_H_

#include <QObject>
#include <QPointer>
#include <QRect>

class QComboBox;
class QVariantAnimation;
class QWidget;
class ElaComboBoxPopup : public QObject
{
    Q_OBJECT
public:
    explicit ElaComboBoxPopup(QComboBox* comboBox);
    void prepare();
    void open(int anchorRow, bool isDropDown = false);
    bool close();
    bool getIsVisible() const;
    bool getIsAnimating() const;
    bool getIsClosing() const;
    bool getIsDropDown() const;
    bool getIsCoveringComboBox() const;
    QRect getBodyRect() const;
    qreal getPopupOpacity() const;
    qreal getShadowOpacity() const;
    qreal getItemChromeOpacity() const;

Q_SIGNALS:
    Q_SIGNAL void closeFinished();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QComboBox* _comboBox{nullptr};                // 关联的组合框主体控件
    QPointer<QWidget> _container;                 // 承载下拉列表的顶层弹出容器
    QVariantAnimation* _revealAnimation{nullptr}; // 驱动展开与收回进度的动画对象
    QRect _fullBodyRect;                          // 容器完整展开时的面板区域
    QRect _comboTargetRect;                       // 组合框主体映射到容器内的目标区域
    QRect _bodyRect;                              // 当前动画帧实际绘制的面板区域
    QRect _startBodyRect;                         // 本次动画起始面板区域
    QRect _endBodyRect;                           // 本次动画目标面板区域
    int _contentOffsetY{0};                       // 列表视图当前纵向像素偏移
    int _startOffsetY{0};                         // 本次动画起始纵向像素偏移
    int _endOffsetY{0};                           // 本次动画目标纵向像素偏移
    qreal _popupOpacity{1.0};                     // 当前面板与内容整体不透明度
    qreal _shadowOpacity{1.0};                    // 当前外围阴影不透明度
    qreal _itemChromeOpacity{1.0};                // 当前列表项高亮与选中指示条不透明度
    qreal _startOpacity{1.0};                     // 收回动画起始面板不透明度
    qreal _startShadowOpacity{1.0};               // 收回动画起始阴影不透明度
    qreal _startChromeOpacity{1.0};               // 收回动画起始列表项装饰不透明度
    bool _isAnimating{false};                     // 当前是否处于展开或收回动画过程中
    bool _isClosing{false};                       // 当前是否处于收回阶段
    bool _isDropDown{false};                      // 是否在主体上下方独立弹出而非原地覆盖
    bool _isOpenUp{false};                        // 独立弹出模式下是否向主体上方展开
    void _resetAnimation();
    void _applyAnimationFrame(qreal progress);
    QRect _getComboTargetRect() const;
    int _getSelectedOffsetY(const QRect& targetRect) const;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELACOMBOBOXPOPUP_H_
