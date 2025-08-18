#ifndef ELARADIOBUTTON_H
#define ELARADIOBUTTON_H

#include <QRadioButton>

#include "ElaProperty.h"
class ElaRadioButtonPrivate;
class ELA_EXPORT ElaRadioButton : public QRadioButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaRadioButton)
public:
    explicit ElaRadioButton(QWidget* parent = nullptr);
    explicit ElaRadioButton(const QString& text, QWidget* parent = nullptr);
    ~ElaRadioButton();
};

#endif // ELARADIOBUTTON_H
