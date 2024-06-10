#ifndef ELATOGGLESWITCH_H
#define ELATOGGLESWITCH_H

#include <QWidget>

#include "stdafx.h"
class ElaToggleSwitchPrivate;
class ELA_EXPORT ElaToggleSwitch : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaToggleSwitch);

public:
    explicit ElaToggleSwitch(QWidget* parent = nullptr);
    ~ElaToggleSwitch();
    void setIsToggled(bool isToggled);
    bool getIsToggled() const;
Q_SIGNALS:
    Q_SIGNAL void toggled(bool checked);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELATOGGLESWITCH_H
