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
    Q_PROPERTY_CREATE_Q_H(ElaIconType::IconName, Awesome)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaLineEdit(QWidget* parent = nullptr);
    explicit ElaLineEdit(ElaIconType::IconName awesome, QWidget* parent = nullptr);
    ~ElaLineEdit();

Q_SIGNALS:
    Q_SIGNAL void focusIn(QString text);
    Q_SIGNAL void focusOut(QString text);
    Q_SIGNAL void wmFocusOut(QString text);

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // ELALINEEDIT_H
