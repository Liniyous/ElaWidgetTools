#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATEACHINGTIPPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATEACHINGTIPPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"

#include <QMargins>
#include <QObject>
#include <QPainterPath>
#include <QPixmap>
#include <QPointer>
#include <QSizeF>

class ElaTeachingTip;
class ElaText;
class ElaToolButton;
class ElaPushButton;
class QScrollArea;
class QVBoxLayout;
class QVariantAnimation;
class QTimer;
class ElaTeachingTipPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaTeachingTip)
public:
    explicit ElaTeachingTipPrivate();
    ~ElaTeachingTipPrivate() override;
    Q_SLOT void onAnimationFinished();
    Q_SLOT void onLayoutTimeout();
    Q_SLOT void onCloseButtonClicked();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QPointer<QWidget> _targetWidget;
    QPointer<QWidget> _customWidget;
    QPointer<QWidget> _animationFocusWidget;
    QPointer<QWidget> _previousFocusWidget;
    QPointer<QWidget> _shadowPressWidget; // 阴影区域按下事件实际对应的下层控件
    QMetaObject::Connection _targetDestroyedConnection;
    QMetaObject::Connection _customDestroyedConnection;
    QScrollArea* _contentScrollArea{nullptr};
    QWidget* _contentWidget{nullptr};
    QWidget* _headerWidget{nullptr};
    QWidget* _buttonWidget{nullptr};
    QVBoxLayout* _contentLayout{nullptr};
    ElaText* _titleText{nullptr};
    ElaText* _subTitleText{nullptr};
    ElaText* _iconText{nullptr};
    ElaToolButton* _closeToolButton{nullptr};
    ElaPushButton* _actionPushButton{nullptr};
    ElaPushButton* _closePushButton{nullptr};
    QVariantAnimation* _animation{nullptr};
    QTimer* _layoutTimer{nullptr};
    QPixmap _animationPix; // 带设备像素比的完整截图 动画期间替代真实内容
    QMargins _shadowMargins; // 主题统一的浮层阴影留白
    QSizeF _animationScale{1, 1};
    QRectF _bodyRect;
    QPointF _animationOrigin; // 尖角顶点 无尖角时为内容中心
    QPainterPath _bubblePath;
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
    ElaTeachingTipType::PlacementMode _preferredPlacement{ElaTeachingTipType::Auto};
    ElaTeachingTipType::PlacementMode _effectivePlacement{ElaTeachingTipType::Auto};
    int _tipWidth{320};
    bool _isTailVisible{true};
    bool _isLightDismissEnabled{false};
    bool _isOpen{false};
    bool _isUpdatingGeometry{false};
    bool _isPresented{false}; // 本次显示尚未发出 closed 通知

    void _updateContent();
    bool _updateGeometry();
    static bool _getIsWidgetWithin(const QWidget* widget, const QWidget* ancestor);
    QRect _getAnchorRect() const;
    QSize _getBodySize(int width) const;
    static Qt::Edge _getTailEdge(ElaTeachingTipType::PlacementMode placement);
    QRect _getCandidateRect(ElaTeachingTipType::PlacementMode placement, const QRect& anchor, const QSize& bodySize) const;
    void _updateBubblePath(const QRect& anchor);
    void _startAnimation(bool isOpening);
    void _restoreContent();
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATEACHINGTIPPRIVATE_H_
