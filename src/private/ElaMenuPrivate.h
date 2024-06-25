#ifndef ELAMENUPRIVATE_H
#define ELAMENUPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaListView;
class ElaMenuModel;
class ElaEvent;
class ElaMenu;
class QAction;
class QLinearGradient;
class ElaMenuDelegate;
class ElaMenuPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMenu)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit ElaMenuPrivate(QObject* parent = nullptr);
    ~ElaMenuPrivate();
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);
    Q_SLOT void onMenuClicked(const QModelIndex& index);
    Q_INVOKABLE void onElaMenuEvent(QVariantMap data);

private:
    QString _menuCheckSumKey{""};
    int _menuItemHeight{35};
    bool _isTopMostMenu{true};
    bool _isCloseAnimationFinished{true};
    int _shadowBorderWidth{6};
    int _menuDepth{0};
    ElaEvent* _menuEvent{nullptr};
    ElaMenu* _parentMenu{nullptr};
    QList<ElaMenu*> _childMenus;
    QList<QAction*> _actionList;
    ElaListView* _centerView{nullptr};
    ElaMenuModel* _centerModel{nullptr};
    ElaMenuDelegate* _centerDelegate{nullptr};
    QLinearGradient* _windowLinearGradient{nullptr};
    bool _containsCursorToAllMenu(ElaMenu* menu, QList<ElaMenu*> menuList);
};

#endif // ELAMENUPRIVATE_H
