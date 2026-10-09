#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADOUBLESPINBOXPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADOUBLESPINBOXPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"

class ElaMenu;
class ElaDoubleSpinBox;
class ElaSpinBoxStyle;
class ElaDoubleSpinBoxPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaDoubleSpinBox)
    ELA_PROPERTY(qreal, ExpandMarkWidth)
public:
    explicit ElaDoubleSpinBoxPrivate(QObject* parent = nullptr);
    ~ElaDoubleSpinBoxPrivate() override;
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaSpinBoxStyle* _style{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    ElaMenu* _createStandardContextMenu();
    void _changeTheme();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELADOUBLESPINBOXPRIVATE_H_

