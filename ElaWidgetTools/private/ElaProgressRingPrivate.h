#ifndef ELAFRAMEWORK_ELAWIDGETTOOLS_PRIVATE_ELAPROGRESSRINGPRIVATE_H_
#define ELAFRAMEWORK_ELAWIDGETTOOLS_PRIVATE_ELAPROGRESSRINGPRIVATE_H_

#include <QObject>

#include "ElaDef.h"
class ElaProgressRing;
class QPropertyAnimation;
class ElaProgressRingPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaProgressRing);
    Q_PROPERTY_CREATE_D(bool, IsBusying)
    Q_PROPERTY_CREATE_D(bool, IsTransparent)
    Q_PROPERTY_CREATE_D(bool, IsDisplayValue)
    Q_PROPERTY_CREATE_D(ElaProgressRingType::ValueDisplayMode, ValueDisplayMode)
    Q_PROPERTY_CREATE_D(int, BusyingWidth)
    Q_PROPERTY_CREATE_D(int, BusyingDurationTime)
    Q_PROPERTY_CREATE_D(int, Minimum)
    Q_PROPERTY_CREATE_D(int, Maximum)
    Q_PROPERTY_CREATE_D(int, Value)
    Q_PROPERTY_CREATE_D(int, ValuePixelSize)
    Q_PROPERTY_CREATE(int, BusyIndex)
    Q_PROPERTY_CREATE(int, BusyStartDeg)
    Q_PROPERTY_CREATE(int, BusyContentDeg)
public:
    explicit ElaProgressRingPrivate(QObject* parent = nullptr);
    ~ElaProgressRingPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
    QPropertyAnimation* _busyStartDegAnimation{nullptr};
    QPropertyAnimation* _busyContentDegAnimation{nullptr};
};

#endif //ELAFRAMEWORK_ELAWIDGETTOOLS_PRIVATE_ELAPROGRESSRINGPRIVATE_H_
