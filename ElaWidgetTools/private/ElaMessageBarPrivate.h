#ifndef ELAMESSAGEBARPRIVATE_H
#define ELAMESSAGEBARPRIVATE_H

#include <QMap>
#include <QObject>
#include <QQueue>
#include <QVariantMap>

#include "ElaDef.h"
#include "ElaSingleton.h"

class ElaIconButton;
class QPainter;
class ElaMessageBar;
class ElaMessageBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMessageBar)
    Q_PROPERTY_CREATE(qreal, Opacity)
    Q_PROPERTY_CREATE(qreal, TimePercent)
public:
    explicit ElaMessageBarPrivate(QObject* parent = nullptr);
    ~ElaMessageBarPrivate() override;
    void onOtherMessageBarEnd();
    Q_SLOT void messageBarEnd();

private:
    friend class ElaMessageBarManager;
    ElaThemeType::ThemeMode _themeMode;
    int _borderRadius{6};
    QString _title{""};
    QString _text{""};
    ElaMessageBarType::PositionPolicy _policy;
    ElaMessageBarType::MessageMode _messageMode;
    qreal _createTime{0};

    // 位置数据
    int _leftPadding{20};                // 左边框到图标中心
    int _titleLeftSpacing{30};           // 图标中心到Title左侧
    int _textLeftSpacing{15};            // Title右侧到Text左侧
    int _closeButtonLeftRightMargin{20}; // closeButton左右总Margin
    int _closeButtonWidth{30};
    int _messageBarHorizontalMargin{20};
    int _messageBarVerticalBottomMargin{20};
    int _messageBarVerticalTopMargin{20};
    int _messageBarSpacing{15};
    int _shadowBorderWidth{6};
    qreal _timePercentHeight{2};

    // 逻辑数据
    bool _isNormalDisplay{false};
    bool _isOtherMessageBarEnd{false};
    ElaIconButton* _closeButton{nullptr};
    void _messageBarCreate(int displayMsec);

    // 初始坐标计算
    void _calculateInitialPos(int& startX, int& startY, int& endX, int& endY);
    //获取总高度和次序信息
    QList<int> _getOtherMessageBarTotalData();
    //计算目标坐标
    qreal _calculateTargetPosY();

    //创建次序判断
    bool _judgeCreateOrder(ElaMessageBar* otherMessageBar);
    void _updateActiveMap(bool isActive);

    // 绘制函数
    void _drawSuccess(QPainter* painter);
    void _drawWarning(QPainter* painter);
    void _drawInformation(QPainter* painter);
    void _drawError(QPainter* painter);
};

#endif // ELAMESSAGEBARPRIVATE_H
