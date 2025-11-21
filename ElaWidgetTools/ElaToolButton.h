#ifndef ELATOOLBUTTON_H
#define ELATOOLBUTTON_H

#include <QToolButton>

#include "ElaDef.h"
class ElaMenu;
class ElaToolButtonPrivate;
class ELA_EXPORT ElaToolButton : public QToolButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaToolButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius);
    Q_PROPERTY_CREATE_Q_H(bool, IsSelected);

public:
    explicit ElaToolButton(QWidget* parent = nullptr);
    ~ElaToolButton() override;

    void setIsTransparent(bool isTransparent);
    bool getIsTransparent() const;

    void setMenu(ElaMenu* menu);
    void setElaIcon(ElaIconType::IconName icon);
    void setElaIcon(ElaIconType::IconName icon, int rotate);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // ELATOOLBUTTON_H
