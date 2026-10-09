#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAICONBUTTONPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAICONBUTTONPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QColor>
#include <QObject>
#include <QPixmap>

#include "ElaWidgetToolsDef.h"
class ElaIconButton;
class ElaIconButtonPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaIconButton)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY_D(qreal, Opacity);
    ELA_PROPERTY_D(ElaIconType::IconName, Awesome)
    ELA_PROPERTY_D(QColor, LightHoverColor);
    ELA_PROPERTY_D(QColor, DarkHoverColor);
    ELA_PROPERTY_D(QColor, LightIconColor);
    ELA_PROPERTY_D(QColor, DarkIconColor);
    ELA_PROPERTY_D(QColor, LightHoverIconColor);
    ELA_PROPERTY_D(QColor, DarkHoverIconColor);
    ELA_PROPERTY_D(bool, IsSelected);
    ELA_PROPERTY(int, HoverAlpha)
public:
    explicit ElaIconButtonPrivate(QObject* parent = nullptr);
    ~ElaIconButtonPrivate();

private:
    QPixmap _iconPix;
    bool _isAlphaAnimationFinished{true};
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAICONBUTTONPRIVATE_H_

