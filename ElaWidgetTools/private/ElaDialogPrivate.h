#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADIALOGPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADIALOGPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaAppBar;
class ElaDialog;
class ElaDialogPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaDialog)
public:
    explicit ElaDialogPrivate(QObject* parent = nullptr);
    ~ElaDialogPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaApplicationType::WindowDisplayMode _windowDisplayMode;
    ElaAppBar* _appBar{nullptr};
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADIALOGPRIVATE_H_

