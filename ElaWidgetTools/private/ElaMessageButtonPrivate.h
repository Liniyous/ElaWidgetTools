#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMESSAGEBUTTONPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMESSAGEBUTTONPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaMessageButton;
class ElaMessageButtonPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaMessageButton)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY_D(QString, BarTitle);
    ELA_PROPERTY_D(QString, BarText);
    ELA_PROPERTY_D(int, DisplayMsec);
    ELA_PROPERTY_D(QWidget*, MessageTargetWidget)
    ELA_PROPERTY_D(ElaMessageBarType::MessageMode, MessageMode);
    ELA_PROPERTY_D(ElaMessageBarType::PositionPolicy, PositionPolicy);

public:
    explicit ElaMessageButtonPrivate(QObject* parent = nullptr);
    ~ElaMessageButtonPrivate();

private:
    bool _isLeftButtonPress{false};
    int _shadowBorderWidth{3};
    int _penBorderWidth{1};
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMESSAGEBUTTONPRIVATE_H_

