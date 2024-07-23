#ifndef ELAWIDGETPRIVATE_H
#define ELAWIDGETPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaWidget;
class ElaAppBar;
class QLinearGradient;
class ElaWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaWidget)
public:
    explicit ElaWidgetPrivate(QObject* parent = nullptr);
    ~ElaWidgetPrivate();
    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaAppBar* _appBar{nullptr};
    QLinearGradient* _windowLinearGradient{nullptr};
};

#endif // ELAWIDGETPRIVATE_H
