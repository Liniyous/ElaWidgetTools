#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASPINBOXPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASPINBOXPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"

class ElaMenu;
class ElaSpinBox;
class ElaSpinBoxStyle;
class ElaSpinBoxPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaSpinBox)
    ELA_PROPERTY(qreal, ExpandMarkWidth)
public:
    explicit ElaSpinBoxPrivate(QObject* parent = nullptr);
    ~ElaSpinBoxPrivate() override;
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaSpinBoxStyle* _style{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    ElaMenu* _createStandardContextMenu();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASPINBOXPRIVATE_H_

