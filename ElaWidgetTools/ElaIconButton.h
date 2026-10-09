#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAICONBUTTON_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAICONBUTTON_H_

#include <QPushButton>

#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaIconButtonPrivate;
class ELA_EXPORT ElaIconButton : public QPushButton
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaIconButton)
    ELA_PROPERTY_Q_H(int, BorderRadius)
    ELA_PROPERTY_Q_H(qreal, Opacity);
    ELA_PROPERTY_Q_H(QColor, LightHoverColor);
    ELA_PROPERTY_Q_H(QColor, DarkHoverColor);
    ELA_PROPERTY_Q_H(QColor, LightIconColor);
    ELA_PROPERTY_Q_H(QColor, DarkIconColor);
    ELA_PROPERTY_Q_H(QColor, LightHoverIconColor);
    ELA_PROPERTY_Q_H(QColor, DarkHoverIconColor);
    ELA_PROPERTY_Q_H(bool, IsSelected);

public:
    ElaIconButton(const QPixmap& pix, QWidget* parent = nullptr);
    ElaIconButton(ElaIconType::IconName awesome, QWidget* parent = nullptr);
    ElaIconButton(ElaIconType::IconName awesome, int pixelSize, QWidget* parent = nullptr);
    ElaIconButton(ElaIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QWidget* parent = nullptr);
    ~ElaIconButton();
    void setAwesome(ElaIconType::IconName awesome);
    ElaIconType::IconName getAwesome() const;

    void setPixmap(const QPixmap& pix);

protected:
    virtual bool event(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAICONBUTTON_H_
