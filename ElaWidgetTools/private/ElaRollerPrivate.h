#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAROLLERPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAROLLERPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QTimer>
class ElaRoller;
class QPropertyAnimation;
class ElaRollerPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaRoller)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY(qreal, ScrollOffset)
    ELA_PROPERTY_D(QStringList, ItemList)
    ELA_PROPERTY_D(int, ItemHeight)
    ELA_PROPERTY_D(int, MaxVisibleItems)
    ELA_PROPERTY_D(int, CurrentIndex)
    ELA_PROPERTY_D(bool, IsContainer)
    ELA_PROPERTY_D(bool, IsEnableLoop)
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

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAROLLERPRIVATE_H_

