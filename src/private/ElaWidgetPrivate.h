#ifndef ELAWIDGETPRIVATE_H
#define ELAWIDGETPRIVATE_H

#include <QObject>
#include <QWKWidgets/widgetwindowagent.h>
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
    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    bool _isEnableMica;
    ElaAppBar* _appBar{nullptr};

    void _doWindowAttributeModeChange(QString mode);
    QWK::WidgetWindowAgent *_windowAgent;
    QString _oldWindowAttribute;
};

#endif // ELAWIDGETPRIVATE_H
