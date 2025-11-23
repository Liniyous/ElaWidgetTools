#ifndef ELAFRAMEWORK_ELAWIDGETTOOLS_PRIVATE_ELAROLLERPRIVATE_H_
#define ELAFRAMEWORK_ELAWIDGETTOOLS_PRIVATE_ELAROLLERPRIVATE_H_

#include "ElaDef.h"
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QTimer>
class ElaRoller;
class QPropertyAnimation;
class ElaRollerPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaRoller)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE(qreal, ScrollOffset)
    Q_PROPERTY_CREATE_D(QStringList, ItemList)
    Q_PROPERTY_CREATE_D(int, ItemHeight)
    Q_PROPERTY_CREATE_D(int, MaxVisibleItems)
    Q_PROPERTY_CREATE_D(int, CurrentIndex)
    Q_PROPERTY_CREATE_D(bool, IsContainer)
    Q_PROPERTY_CREATE_D(bool, IsEnableLoop)
public:
    explicit ElaRollerPrivate(QObject* parent = nullptr);
    ~ElaRollerPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
    QPropertyAnimation* _scrollAnimation{nullptr};
    int _penBorderWidth{1};
    int _indicatorMargin{3};
    bool _isUpArrowPress{false};
    bool _isDownArrowPress{false};
    QRect _upArrowRect;
    QRect _downArrowRect;
    QTimer* _pressSustainTimer{nullptr};
    QTimer* _repeatScrollTimer{nullptr};
    qreal _targetScrollOffset{0};
    QPoint _mousePoint;
    void _scroll(int delta);
};

#endif //ELAFRAMEWORK_ELAWIDGETTOOLS_PRIVATE_ELAROLLERPRIVATE_H_
