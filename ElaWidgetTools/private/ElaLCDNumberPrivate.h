#ifndef ELAWIDGETTOOLS_ELALCDNUMBERPRIVATE_H
#define ELAWIDGETTOOLS_ELALCDNUMBERPRIVATE_H

#include "ElaDef.h"

#include <QObject>
class QTimer;
class ElaLCDNumber;
class ElaLCDNumberStyle;
class ElaLCDNumberPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaLCDNumber)
    Q_PROPERTY_CREATE_D(bool, IsUseAutoClock)
    Q_PROPERTY_CREATE_D(QString, AutoClockFormat)
public:
    explicit ElaLCDNumberPrivate(QObject* parent = nullptr);
    ~ElaLCDNumberPrivate() override;

    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);

private:
    QTimer* _clockTimer{nullptr};
    ElaLCDNumberStyle* _lcdNumberStyle{nullptr};
};

#endif //ELAWIDGETTOOLS_ELALCDNUMBERPRIVATE_H
