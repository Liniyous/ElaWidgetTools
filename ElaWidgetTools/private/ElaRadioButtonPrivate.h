#ifndef ELARADIOBUTTONPRIVATE_H
#define ELARADIOBUTTONPRIVATE_H
#include <QObject>

#include "ElaDef.h"
class ElaRadioButton;
class ElaRadioButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaRadioButton)
public:
    explicit ElaRadioButtonPrivate(QObject* parent = nullptr);
    ~ElaRadioButtonPrivate();
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);
};

#endif // ELARADIOBUTTONPRIVATE_H
