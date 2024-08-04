#include "ElaToolButton.h"

#include "DeveloperComponents/ElaToolButtonStyle.h"
#include "ElaIcon.h"
#include "ElaMenu.h"
#include "ElaToolButtonPrivate.h"
ElaToolButton::ElaToolButton(QWidget* parent)
    : QToolButton(parent), d_ptr(new ElaToolButtonPrivate())
{
    Q_D(ElaToolButton);
    d->q_ptr = this;
    setIconSize(QSize(22, 22));
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    setPopupMode(QToolButton::InstantPopup);
    setStyle(new ElaToolButtonStyle(style()));
}

ElaToolButton::~ElaToolButton()
{
}

void ElaToolButton::setMenu(ElaMenu* menu)
{
    if (!menu || menu == this->menu())
    {
        return;
    }
    menu->setMenuItemHeight(27);
    QToolButton::setMenu(menu);
}

void ElaToolButton::setElaIcon(ElaIconType icon)
{
    setProperty("ElaIconType", QChar((unsigned short)icon));
    setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Broom, 1));
}
