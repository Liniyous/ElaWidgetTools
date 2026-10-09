#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAWIDGETPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAWIDGETPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaWidget;
class ElaAppBar;
class ElaWidgetPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaWidget)
public:
    explicit ElaWidgetPrivate(QObject* parent = nullptr);
    ~ElaWidgetPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaApplicationType::WindowDisplayMode _windowDisplayMode;
    ElaAppBar* _appBar{nullptr};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAWIDGETPRIVATE_H_

