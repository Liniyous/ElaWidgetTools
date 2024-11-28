#ifndef ELAWIDGETTOOLS_SRC_PRIVATE_ELAKEYBINDERPRIVATE_H_
#define ELAWIDGETTOOLS_SRC_PRIVATE_ELAKEYBINDERPRIVATE_H_

#include "Def.h"
#include <QObject>
class ElaKeyBinder;
class ElaContentDialog;
class ElaKeyBinderContainer;
class ElaKeyBinderPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaKeyBinder)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit ElaKeyBinderPrivate(QObject* parent = nullptr);
    ~ElaKeyBinderPrivate();
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaContentDialog* _binderDialog{nullptr};
    ElaKeyBinderContainer* _binderContainer{nullptr};
};

#endif//ELAWIDGETTOOLS_SRC_PRIVATE_ELAKEYBINDERPRIVATE_H_
