#ifndef ELADOUBLESPINBOXPRIVATE_H
#define ELADOUBLESPINBOXPRIVATE_H

#include <QObject>

#include "Def.h"

class ElaMenu;
class ElaDoubleSpinBox;
class ElaSpinBoxStyle;
class ElaDoubleSpinBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaDoubleSpinBox)
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
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

#endif // ELADOUBLESPINBOXPRIVATE_H
