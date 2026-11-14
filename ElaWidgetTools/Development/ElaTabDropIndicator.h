#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATABDROPINDICATOR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATABDROPINDICATOR_H_

#include <QWidget>

class QPropertyAnimation;

// 标签拖放区域的主题覆盖与过渡动画
class ElaTabDropIndicator : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal pOpacity READ getOpacity WRITE setOpacity)
public:
    explicit ElaTabDropIndicator(QWidget* parent);
    void showTarget(const QRect& target);
    void hideTarget(bool animated = true);
    qreal getOpacity() const;
    void setOpacity(qreal opacity);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void _animateOpacity(qreal opacity);

    qreal _opacity{0};                               // 覆盖颜色相对于主题透明度的比例
    QPropertyAnimation* _geometryAnimation{nullptr}; // 拖放区域之间的尺寸过渡
    QPropertyAnimation* _opacityAnimation{nullptr};  // 覆盖效果的淡入淡出
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELATABDROPINDICATOR_H_
