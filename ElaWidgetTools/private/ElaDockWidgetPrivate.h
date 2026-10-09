#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADOCKWIDGETPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADOCKWIDGETPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaDockWidget;
class ElaDockWidgetTitleBar;
class ElaDockWidgetPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaDockWidget)
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

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADOCKWIDGETPRIVATE_H_

