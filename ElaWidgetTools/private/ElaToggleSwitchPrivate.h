#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOGGLESWITCHPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOGGLESWITCHPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaToggleSwitch;
class ElaToggleSwitchPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaToggleSwitch);

public:
    explicit ElaToggleSwitchPrivate(QObject* parent = nullptr);
    ~ElaToggleSwitchPrivate();

private:
    bool _isToggled{false};
    int _margin{1};
    qreal _circleCenterX{0};
    qreal _circleRadius{0};
    bool _isLeftButtonPress{false};
    bool _isMousePressMove{false};
    int _lastMouseX{0};
    ElaThemeType::ThemeMode _themeMode;
    void _startPosAnimation(qreal startX, qreal endX, bool isToggle);
    void _startRadiusAnimation(qreal startRadius, qreal endRadius);
    void _adjustCircleCenterX();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOGGLESWITCHPRIVATE_H_

