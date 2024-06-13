#ifndef ELAWIDGETPRIVATE_H
#define ELAWIDGETPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaWidget;
class ElaAppBar;
class QLinearGradient;
class QVBoxLayout;
class ElaWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaWidget)
public:
    explicit ElaWidgetPrivate(QObject* parent = nullptr);
    ~ElaWidgetPrivate();
    Q_SLOT void onThemeModeChanged(ElaApplicationType::ThemeMode themeMode);

private:
    ElaAppBar* _appBar{nullptr};
    QLinearGradient* _windowLinearGradient{nullptr};
    QWidget* _centralWidget{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
};

#endif // ELAWIDGETPRIVATE_H
