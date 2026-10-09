#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPUSHBUTTONPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPUSHBUTTONPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QColor>
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaPushButton;
class ElaPushButtonPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaPushButton)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY_D(QColor, LightDefaultColor)
    ELA_PROPERTY_D(QColor, DarkDefaultColor)
    ELA_PROPERTY_D(QColor, LightHoverColor)
    ELA_PROPERTY_D(QColor, DarkHoverColor)
    ELA_PROPERTY_D(QColor, LightPressColor)
    ELA_PROPERTY_D(QColor, DarkPressColor)
public:
    explicit ElaPushButtonPrivate(QObject* parent = nullptr);
    ~ElaPushButtonPrivate();

private:
    QColor _lightTextColor;
    QColor _darkTextColor;
    bool _isPressed{false};
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAPUSHBUTTONPRIVATE_H_
