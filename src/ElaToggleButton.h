#ifndef ELATOGGLEBUTTON_H
#define ELATOGGLEBUTTON_H

#include <QPushButton>

#include "stdafx.h"
class ElaToggleButtonPrivate;
class ELA_EXPORT ElaToggleButton : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaToggleButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaToggleButton(QWidget* parent = nullptr);
    explicit ElaToggleButton(QString text, QWidget* parent = nullptr);
    ~ElaToggleButton();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELATOGGLEBUTTON_H
