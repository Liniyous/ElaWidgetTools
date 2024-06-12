#ifndef ELAMESSAGEBARPRIVATE_H
#define ELAMESSAGEBARPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaIconButton;
class ElaMessageBar;
class QPainter;
class ElaMessageBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMessageBar)
public:
    explicit ElaMessageBarPrivate(QObject* parent = nullptr);
    ~ElaMessageBarPrivate();
    Q_SLOT void _onCloseButtonClicked();
    Q_SLOT void onOtherMessageBarClosed(ElaMessageBarType::PositionPolicy policy, int messageBarIndex);
Q_SIGNALS:
    Q_SIGNAL void messageBarClosed(ElaMessageBarType::PositionPolicy policy, int messageBarIndex);

private:
    ElaApplicationType::ThemeMode _themeMode;
    int _borderRadius{6};
    QString _title{""};
    QString _text{""};
    ElaMessageBarType::PositionPolicy _policy;
    ElaMessageBarType::MessageMode _messageMode;

    // 位置数据
    int _leftPadding{20};                // 左边框到图标中心
    int _titleLeftSpacing{30};           // 图标中心到Title左侧
    int _textLeftSpacing{15};            // Title右侧到Text左侧
    int _closeButtonLeftRightMargin{20}; // closeButton左右总Margin
    int _closeButtonWidth{30};
    int _messageBarHorizontalMargin{20};
    int _messageBarVerticalBottomMargin{20};
    int _messageBarVerticalTopMargin{50};
    int _messageBarSpacing{15};
    int _shadowBorderWidth{6};

    // 逻辑数据
    int _messageBarIndex{0};
    bool _isMessageBarStartAnimationFinished{false};
    bool _isMessageBarEventAnimationInStartAnimation{false};
    bool _isCloseAnimationStart{false};
    bool _isNormalDisplay{false};
    bool _isMessageBarEventAnimationStart{false};
    ElaIconButton* _closeButton{nullptr};
    static QMap<ElaMessageBarType::PositionPolicy, QList<ElaMessageBar*>*> _messageBarActiveMap;
    void _messageBarStartAnimation(int displayMsec);
    void _messageBarFinshAnimation();
    void _messageBarEventAnimation();
    void _updateActiveMap(bool isActive);

    // 初始坐标计算
    void _calculatePos(int& startX, int& startY, int& endX, int& endY);
    int _calculateMinimumHeightTotal(bool isJudgeIndex = false);

    // 绘制函数
    void _drawSuccess(QPainter* painter);
    void _drawWarning(QPainter* painter);
    void _drawInformation(QPainter* painter);
    void _drawError(QPainter* painter);
};

#endif // ELAMESSAGEBARPRIVATE_H
