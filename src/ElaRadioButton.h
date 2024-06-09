#ifndef ELARADIOBUTTON_H
#define ELARADIOBUTTON_H

#include <QRadioButton>

#include "Def.h"
class ElaRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit ElaRadioButton(QWidget* parent = nullptr);
    explicit ElaRadioButton(const QString& text, QWidget* parent = nullptr);
    ~ElaRadioButton();
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);
};

#endif // ELARADIOBUTTON_H
