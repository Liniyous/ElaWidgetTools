#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATOOLBUTTON_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATOOLBUTTON_H_

#include "ElaPropertyMacro.h"
#include <QToolButton>

#include "ElaWidgetToolsDef.h"
class ElaMenu;
class ElaToolButtonPrivate;
class ELA_EXPORT ElaToolButton : public QToolButton
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaToolButton)
public:
    explicit ElaToolButton(QWidget* parent = nullptr);
    ~ElaToolButton() override;

    void setBorderRadius(int borderRadius);
    Q_REQUIRED_RESULT int getBorderRadius() const;
    void setIsSelected(bool isSelected);
    Q_REQUIRED_RESULT bool getIsSelected() const;

    void setIsTransparent(bool isTransparent);
    bool getIsTransparent() const;

    void setMenu(ElaMenu* menu);
    void setElaIcon(ElaIconType::IconName icon);
    void setElaIcon(ElaIconType::IconName icon, int rotate);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

Q_SIGNALS:
    Q_SIGNAL void pBorderRadiusChanged();
    Q_SIGNAL void pIsSelectedChanged();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELATOOLBUTTON_H_
