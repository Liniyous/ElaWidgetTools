#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOGGLEBUTTONPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOGGLEBUTTONPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaToggleButton;
class ElaToggleButtonPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaToggleButton)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY_D(QString, Text)
    ELA_PROPERTY(int, ToggleAlpha)
public:
    explicit ElaToggleButtonPrivate(QObject* parent = nullptr);
    ~ElaToggleButtonPrivate() override;

private:
    bool _isAlphaAnimationFinished{true};
    bool _isToggled{false};
    bool _isPressed{false};
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOGGLEBUTTONPRIVATE_H_

