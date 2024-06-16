#ifndef ELACOMBOBOXPRIVATE_H
#define ELACOMBOBOXPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaComboBox;
class ElaComboBoxView;
class ElaComboBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaComboBox);
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit ElaComboBoxPrivate(QObject* parent = nullptr);
    ~ElaComboBoxPrivate();

private:
    ElaComboBoxView* _comboView{nullptr};
    bool _isHidePopupAnimationFinished{true};
    ElaApplicationType::ThemeMode _themeMode;
};

#endif // ELACOMBOBOXPRIVATE_H
