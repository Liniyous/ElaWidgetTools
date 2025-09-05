#ifndef ELACOMBOBOXPRIVATE_H
#define ELACOMBOBOXPRIVATE_H

#include <QObject>

#include "ElaDef.h"

class QLineEdit;
class ElaComboBox;
class ElaComboBoxStyle;
class ElaComboBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaComboBox);
    Q_PROPERTY_CREATE_D(int, BorderRadius)

public:
    explicit ElaComboBoxPrivate(QObject* parent = nullptr);
    ~ElaComboBoxPrivate() override;

    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    bool _isAllowHidePopup{false};
    ElaComboBoxStyle* _comboBoxStyle{nullptr};
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELACOMBOBOXPRIVATE_H
