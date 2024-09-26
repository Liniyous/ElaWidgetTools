#ifndef ELAWIDGETPRIVATE_H
#define ELAWIDGETPRIVATE_H

#include <QObject>

#include "Def.h"
class ElaWidget;
class ElaAppBar;
class ElaWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaWidget)
public:
    explicit ElaWidgetPrivate(QObject* parent = nullptr);
    ~ElaWidgetPrivate();

private:
    ElaThemeType::ThemeMode _themeMode;
    bool _isEnableMica;
    ElaAppBar* _appBar{nullptr};
};

#endif // ELAWIDGETPRIVATE_H
