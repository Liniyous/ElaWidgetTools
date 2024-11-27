#ifndef ELALOADBUTTON_H
#define ELALOADBUTTON_H

#include <QPushButton>

#include "stdafx.h"
#include "ElaProgressRing.h"
class ElaLoadButtonPrivate;
class ELA_EXPORT ElaLoadButton : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaLoadButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkPressColor)
public:
    explicit ElaLoadButton(QWidget* parent = nullptr);
    explicit ElaLoadButton(QString text, QWidget* parent = nullptr);
    ~ElaLoadButton();

    void setLightTextColor(QColor color);
    QColor getLightTextColor() const;

    void setDarkTextColor(QColor color);
    QColor getDarkTextColor() const;

    void setText(const QString &text);
    void setEnabled(bool enabled);
    void setLoading(bool loading, int waittime=5000);

Q_SIGNALS:
    Q_SIGNAL void timeout();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELALOADBUTTON_H
