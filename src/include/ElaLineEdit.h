#ifndef ELALINEEDIT_H
#define ELALINEEDIT_H

#include <QLineEdit>

#include "Def.h"
#include "stdafx.h"

class ElaLineEditPrivate;
class ELA_EXPORT ElaLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_Q_CREATE(ElaLineEdit)
    Q_PROPERTY_CREATE_Q_H(ElaIconType, Awesome)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaLineEdit(QWidget* parent = nullptr);
    ElaLineEdit(ElaIconType awesome, QWidget* parent = nullptr);
    ~ElaLineEdit();

Q_SIGNALS:
    Q_SIGNAL void focusIn(QString text);

protected:
    void focusInEvent(QFocusEvent* event) override;
};

#endif // ELALINEEDIT_H
