#ifndef ELAPUSHBUTTON_H
#define ELAPUSHBUTTON_H

#include <QPushButton>

#include "Def.h"
#include "stdafx.h"
class ELA_EXPORT ElaPushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, BorderRadius)
    Q_PROPERTY_CREATE(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE(QColor, LightHoverColor)
    Q_PROPERTY_CREATE(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE(QColor, LightPressColor)
    Q_PROPERTY_CREATE(QColor, DarkPressColor)
public:
    explicit ElaPushButton(QWidget* parent = nullptr);
    explicit ElaPushButton(QString text, QWidget* parent = nullptr);
    ~ElaPushButton();

    void setLightTextColor(QColor color);
    QColor getLightTextColor() const;

    void setDarkTextColor(QColor color);
    QColor getDarkTextColor() const;

    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QColor _lightTextColor;
    QColor _darkTextColor;
    bool _isPressed{false};
    int _shadowBorderWidth{3};
    ElaApplicationType::ThemeMode _themeMode;
};

#endif // ELAPUSHBUTTON_H
