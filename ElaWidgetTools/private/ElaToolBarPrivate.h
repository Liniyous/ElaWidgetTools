#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOOLBARPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOOLBARPRIVATE_H_
#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaToolBarStyle;
class ElaToolBar;
class ElaToolBarPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaToolBar)
public:
    explicit ElaToolBarPrivate(QObject* parent = nullptr);
    ~ElaToolBarPrivate() override;

private:
    ElaToolBarStyle* _toolBarStyle{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    int _shadowBorderWidth{6};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATOOLBARPRIVATE_H_

