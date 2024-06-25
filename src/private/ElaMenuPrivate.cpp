#include "ElaMenuPrivate.h"

#include <QAction>
#include <QLinearGradient>
#include <QModelIndex>

#include "ElaApplication.h"
#include "ElaEventBus.h"
#include "ElaListView.h"
#include "ElaMenu.h"
ElaMenuPrivate::ElaMenuPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaMenuPrivate::~ElaMenuPrivate()
{
}

void ElaMenuPrivate::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    if (themeMode == ElaApplicationType::Light)
    {
        _windowLinearGradient->setColorAt(0, QColor(0xF2, 0xE7, 0xF5));
        _windowLinearGradient->setColorAt(1, QColor(0xED, 0xF3, 0xF9));
    }
    else
    {
        _windowLinearGradient->setColorAt(0, QColor(0x2D, 0x2D, 0x2D));
        _windowLinearGradient->setColorAt(1, QColor(0x3D, 0x3D, 0x3D));
    }
}

void ElaMenuPrivate::onMenuClicked(const QModelIndex& index)
{
    Q_Q(ElaMenu);
    Q_EMIT _actionList[index.row()]->triggered();
    ElaMenu* childMenu = _actionList.at(index.row())->data().value<ElaMenu*>();
    if (childMenu)
    {
        if (childMenu->d_ptr->_actionList.count() > 0)
        {
            if (!childMenu->isVisible())
            {
                _centerView->clearSelection();
                _centerView->selectionModel()->select(index, QItemSelectionModel::Select);
                QVariantMap postData;
                postData.insert("ElaMenuCheckSumKey", _menuCheckSumKey);
                postData.insert("PopupChildMenu", QVariant::fromValue(childMenu));
                postData.insert("PopupChildMenuDepth", childMenu->d_ptr->_menuDepth);
                ElaEventBus::getInstance()->post("ElaMenuEvent", postData);
                childMenu->popup(q->mapToGlobal(QPoint(180, _menuItemHeight * index.row())));
            }
        }
        return;
    }
    else
    {
        QVariantMap postData;
        postData.insert("ElaMenuCheckSumKey", _menuCheckSumKey);
        postData.insert("CloseAllMenu", "");
        ElaEventBus::getInstance()->post("ElaMenuEvent", postData);
    }
}

void ElaMenuPrivate::onElaMenuEvent(QVariantMap data)
{
    Q_Q(ElaMenu);
    if (_menuCheckSumKey != data.value("ElaMenuCheckSumKey").toString())
    {
        return;
    }
    if (data.contains("CloseAllMenu"))
    {
        if (q->isVisible())
        {
            q->close();
        }
    }
    else if (data.contains("HideAllMenu"))
    {
        if (q->isVisible())
        {
            q->hide();
        }
    }
    else if (data.contains("PopupChildMenu"))
    {
        if (_isTopMostMenu)
        {
            return;
        }
        ElaMenu* popupMenu = data.value("PopupChildMenu").value<ElaMenu*>();
        int depth = data.value("PopupChildMenuDepth").toInt();
        if (popupMenu == q || _menuDepth < depth)
        {
            return;
        }
        q->hide();
    }
}

bool ElaMenuPrivate::_containsCursorToAllMenu(ElaMenu* menu, QList<ElaMenu*> menuList)
{
    if (!menu)
    {
        return false;
    }
    if (ElaApplication::containsCursorToItem(menu))
    {
        return true;
    }
    for (int i = 0; i < menuList.count(); i++)
    {
        auto childMenu = menuList.at(i);
        if (ElaApplication::containsCursorToItem(childMenu))
        {
            return true;
        }
        if (_containsCursorToAllMenu(childMenu, childMenu->d_ptr->_childMenus))
        {
            return true;
        }
    }
    return false;
}
