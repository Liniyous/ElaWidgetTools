#ifndef ELACOMBOBOXVIEWPRIVATE_H
#define ELACOMBOBOXVIEWPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaComboBoxView;
class ElaComboBoxViewPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaComboBoxView)
public:
    explicit ElaComboBoxViewPrivate(QObject* parent = nullptr);
    ~ElaComboBoxViewPrivate();
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);

private:
    bool _isMultiSelect{false};
};

#endif // ELACOMBOBOXVIEWPRIVATE_H
