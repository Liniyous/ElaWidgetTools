#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATEXTPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATEXTPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaText;
class ElaTextPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaText)
    ELA_PROPERTY_D(ElaTextType::TextStyle, TextStyle)
    ELA_PROPERTY_D(ElaIconType::IconName, ElaIcon)
public:
    explicit ElaTextPrivate(QObject* parent = nullptr);
    ~ElaTextPrivate() override;
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    bool _isWrapAnywhere{false};
    qreal _textSpacing{0.5};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATEXTPRIVATE_H_

