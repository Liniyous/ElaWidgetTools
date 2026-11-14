#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLANIMATION_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLANIMATION_H_

#include "ElaWidgetToolsExport.h"

#include <QAbstractAnimation>
#include <QPointF>

// 共享的像素惯性运动 内部控件负责输入换算与可见位置同步
class ELA_EXPORT ElaScrollAnimation : public QAbstractAnimation
{
    Q_OBJECT
public:
    explicit ElaScrollAnimation(QObject* parent = nullptr);
    int duration() const override;
    QPointF getPosition() const;
    QPointF getTargetPosition() const;
    bool scrollBy(const QPointF& movement, const QPointF& position, const QPointF& minimum, const QPointF& maximum, qreal speedLimit = 20);

Q_SIGNALS:
    Q_SIGNAL void positionChanged(QPointF position);

protected:
    void updateCurrentTime(int currentTime) override;

private:
    static constexpr qreal _velocityPerPixel{7.600855902349023}; // WinUI ScrollView 每像素的速度换算系数
    static constexpr qreal _minimumVelocity{30};                 // 惯性截止速度 单位为像素每秒
    static qreal _getTarget(qreal movement, qreal current, qreal pending, qreal minimum, qreal maximum, qreal speedLimit);
    static qreal _getDuration(qreal distance);
    static qreal _getPosition(qreal start, qreal target, qreal elapsed);

    QPointF _startPosition;  // 本次速度衔接时的像素位置
    QPointF _targetPosition; // 惯性终点的像素位置
    QPointF _position;       // 当前帧的像素位置
    int _duration{0};        // 当前惯性运动的总毫秒数
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELASCROLLANIMATION_H_
