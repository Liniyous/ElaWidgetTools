#include "ElaMenuBar.h"

#include <QEvent>
#include <QPainter>
#include <QToolButton>

#include "ElaIcon.h"
#include "ElaMenu.h"
#include "ElaMenuBarStyle.h"
ElaMenuBar::ElaMenuBar(QWidget* parent)
    : QMenuBar(parent)
{
    setMouseTracking(true);
    setObjectName("ElaMenuBar");
    setStyle(new ElaMenuBarStyle(style()));
    QToolButton* tool = this->findChild<QToolButton*>();
    if (tool->objectName() == "qt_menubar_ext_button")
    {
        QMenu* oldMenu = tool->menu();
        ElaMenu* menu = new ElaMenu(this);
        menu->setObjectName("ElaExtendMenu");
        menu->setMenuItemHeight(27);
        if (oldMenu)
        {
            oldMenu->deleteLater();
        }
        tool->setMenu(menu);
    }
}

ElaMenuBar::~ElaMenuBar()
{
    delete this->style();
}

QAction* ElaMenuBar::addMenu(QMenu* menu)
{
    ElaMenu* elaMenu = dynamic_cast<ElaMenu*>(menu);
    if (elaMenu)
    {
        elaMenu->setMenuItemHeight(27);
    }
    return QMenuBar::addMenu(menu);
}

ElaMenu* ElaMenuBar::addMenu(const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    menu->setMenuItemHeight(27);
    QMenuBar::addAction(menu->menuAction());
    return menu;
}

ElaMenu* ElaMenuBar::addMenu(const QIcon& icon, const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    menu->setMenuItemHeight(27);
    menu->setIcon(icon);
    QMenuBar::addAction(menu->menuAction());
    return menu;
}

ElaMenu* ElaMenuBar::addMenu(ElaIconType::IconName icon, const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    menu->setMenuItemHeight(27);
    menu->menuAction()->setProperty("ElaIconType", QChar((unsigned short)icon));
    menu->menuAction()->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Broom, 1));
    QMenuBar::addAction(menu->menuAction());
    return menu;
}

QAction* ElaMenuBar::addElaIconAction(ElaIconType::IconName icon, const QString& text)
{
    QAction* action = new QAction(text, this);
    action->setProperty("ElaIconType", QChar((unsigned short)icon));
    action->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Broom, 1));
    QMenuBar::addAction(action);
    return action;
}
