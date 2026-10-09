#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAPUSHBUTTON_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAPUSHBUTTON_H_

#include <QPushButton>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaPushButtonPrivate;
class ELA_EXPORT ElaPushButton : public QPushButton
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaPushButton)
    ELA_PROPERTY_Q_H(int, BorderRadius)
    ELA_PROPERTY_Q_H(QColor, LightDefaultColor)
    ELA_PROPERTY_Q_H(QColor, DarkDefaultColor)
    ELA_PROPERTY_Q_H(QColor, LightHoverColor)
    ELA_PROPERTY_Q_H(QColor, DarkHoverColor)
    ELA_PROPERTY_Q_H(QColor, LightPressColor)
    ELA_PROPERTY_Q_H(QColor, DarkPressColor)
public:
    explicit ElaPushButton(QWidget* parent = nullptr);
    explicit ElaPushButton(const QString& text, QWidget* parent = nullptr);
    ~ElaPushButton();

    void setLightTextColor(const QColor& color);
    const QColor& getLightTextColor() const;

    void setDarkTextColor(const QColor& color);
    const QColor& getDarkTextColor() const;

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAPUSHBUTTON_H_
