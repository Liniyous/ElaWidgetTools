#ifndef ELALINEEDIT_H
#define ELALINEEDIT_H

#include <QLineEdit>

#include "stdafx.h"

class ElaLineEditPrivate;
class ELA_EXPORT ElaLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_Q_CREATE(ElaLineEdit)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(bool, IsClearButtonEnable)
public:
    explicit ElaLineEdit(QWidget* parent = nullptr);
    ~ElaLineEdit() override;

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
