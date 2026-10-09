#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPROGRESSRINGPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPROGRESSRINGPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaProgressRing;
class QPropertyAnimation;
class ElaProgressRingPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaProgressRing);
    ELA_PROPERTY_D(bool, IsBusying)
    ELA_PROPERTY_D(bool, IsTransparent)
    ELA_PROPERTY_D(bool, IsDisplayValue)
    ELA_PROPERTY_D(ElaProgressRingType::ValueDisplayMode, ValueDisplayMode)
    ELA_PROPERTY_D(int, BusyingWidth)
    ELA_PROPERTY_D(int, BusyingDurationTime)
    ELA_PROPERTY_D(int, Minimum)
    ELA_PROPERTY_D(int, Maximum)
    ELA_PROPERTY_D(int, Value)
    ELA_PROPERTY_D(int, ValuePixelSize)
    ELA_PROPERTY(int, BusyIndex)
    ELA_PROPERTY(int, BusyStartDeg)
    ELA_PROPERTY(int, BusyContentDeg)
public:
    explicit ElaProgressRingPrivate(QObject* parent = nullptr);
    ~ElaProgressRingPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
    QPropertyAnimation* _busyStartDegAnimation{nullptr};
    QPropertyAnimation* _busyContentDegAnimation{nullptr};
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPROGRESSRINGPRIVATE_H_

