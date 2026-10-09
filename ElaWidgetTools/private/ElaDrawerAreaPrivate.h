#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADRAWERAREAPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADRAWERAREAPRIVATE_H_

#include "ElaDrawerContainer.h"
#include "ElaDrawerHeader.h"
#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"
#include <QObject>
class ElaDrawerArea;
class ElaDrawerAreaPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaDrawerArea)
public:
    explicit ElaDrawerAreaPrivate(QObject* parent = nullptr);
    ~ElaDrawerAreaPrivate() override;

    Q_SLOT void onDrawerHeaderClicked(bool isExpand);

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaDrawerHeader* _drawerHeader{nullptr};
    ElaDrawerContainer* _drawerContainer{nullptr};
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADRAWERAREAPRIVATE_H_

