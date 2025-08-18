#ifndef ELADOCKWIDGETPRIVATE_H
#define ELADOCKWIDGETPRIVATE_H

#include <QObject>

#include "ElaDef.h"
class ElaDockWidget;
class ElaDockWidgetTitleBar;
class ElaDockWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaDockWidget)
public:
    explicit ElaDockWidgetPrivate(QObject* parent = nullptr);
    ~ElaDockWidgetPrivate();
    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);

private:
    qint64 _currentWinID{0};
    int _margins{8};
    ElaThemeType::ThemeMode _themeMode;
    ElaApplicationType::WindowDisplayMode _windowDisplayMode;
    int _shadowBorderWidth{6};
    ElaDockWidgetTitleBar* _titleBar{nullptr};
};

#endif // ELADOCKWIDGETPRIVATE_H
