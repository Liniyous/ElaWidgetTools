#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALCDNUMBERPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALCDNUMBERPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"

#include <QObject>
class QTimer;
class ElaLCDNumber;
class ElaLCDNumberStyle;
class ElaLCDNumberPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaLCDNumber)
    ELA_PROPERTY_D(bool, IsUseAutoClock)
    ELA_PROPERTY_D(QString, AutoClockFormat)
public:
    explicit ElaLCDNumberPrivate(QObject* parent = nullptr);
    ~ElaLCDNumberPrivate() override;

    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    QTimer* _clockTimer{nullptr};
    ElaLCDNumberStyle* _lcdNumberStyle{nullptr};
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELALCDNUMBERPRIVATE_H_

