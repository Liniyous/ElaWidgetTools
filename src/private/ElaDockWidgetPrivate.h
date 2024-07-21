#ifndef ELADOCKWIDGETPRIVATE_H
#define ELADOCKWIDGETPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaDockWidget;
class QLinearGradient;
class ElaDockWidgetTitleBar;
class ElaDockWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaDockWidget)
public:
    explicit ElaDockWidgetPrivate(QObject* parent = nullptr);
    ~ElaDockWidgetPrivate();
    Q_SLOT void onThemeModeChanged(ElaApplicationType::ThemeMode themeMode);

private:
    ElaApplicationType::ThemeMode _themeMode;
    int _shadowBorderWidth{6};
    QLinearGradient* _windowLinearGradient{nullptr};
    ElaDockWidgetTitleBar* _titleBar{nullptr};
};

#endif // ELADOCKWIDGETPRIVATE_H
