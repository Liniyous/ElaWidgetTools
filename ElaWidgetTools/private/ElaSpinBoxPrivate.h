#ifndef ELASPINBOXPRIVATE_H
#define ELASPINBOXPRIVATE_H

#include <QObject>

#include "ElaDef.h"

class ElaMenu;
class ElaSpinBox;
class ElaSpinBoxStyle;
class ElaSpinBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaSpinBox)
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
public:
    explicit ElaSpinBoxPrivate(QObject* parent = nullptr);
    ~ElaSpinBoxPrivate() override;
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaSpinBoxStyle* _style{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    ElaMenu* _createStandardContextMenu();
};

#endif // ELASPINBOXPRIVATE_H
