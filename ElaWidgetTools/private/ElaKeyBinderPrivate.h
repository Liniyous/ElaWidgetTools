#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAKEYBINDERPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAKEYBINDERPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include <QObject>
class ElaKeyBinder;
class ElaContentDialog;
class ElaKeyBinderContainer;
class ElaKeyBinderPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaKeyBinder)
    ELA_PROPERTY_D(int, BorderRadius)
public:
    explicit ElaKeyBinderPrivate(QObject* parent = nullptr);
    ~ElaKeyBinderPrivate() override;
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaContentDialog* _binderDialog{nullptr};
    ElaKeyBinderContainer* _binderContainer{nullptr};
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAKEYBINDERPRIVATE_H_

