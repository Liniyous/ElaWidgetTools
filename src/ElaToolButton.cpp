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
    connect(menu, &ElaMenu::menuShow, this, [=]() {
        QPoint pos = menu->pos();
        if (pos.y() + menu->getMenuItemHeight() + 9 >= QCursor::pos().y())
        {
            menu->move(pos.x() - abs(menu->width() - this->width()) / 2, pos.y() + 5);
        }
        else
        {
            menu->move(pos.x() - abs(menu->width() - this->width()) / 2, pos.y() - 5);
        }
    });
    QToolButton::setMenu(menu);
}

void ElaToolButton::setElaIcon(ElaIconType icon)
{
    setProperty("ElaIconType", QChar((unsigned short)icon));
    setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Broom, 1));
}
