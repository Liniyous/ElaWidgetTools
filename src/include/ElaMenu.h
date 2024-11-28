#ifndef ELAMENU_H
#define ELAMENU_H

#include <QMenu>
#include <QWidget>

#include "Def.h"
#include "stdafx.h"
class ElaMenuPrivate;
class ELA_EXPORT ElaMenu : public QMenu
{
    Q_OBJECT
    Q_Q_CREATE(ElaMenu)

public:
    explicit ElaMenu(QWidget* parent = nullptr);
    explicit ElaMenu(const QString& title, QWidget* parent = nullptr);
    ~ElaMenu();
    void setMenuItemHeight(int menuItemHeight);
    int getMenuItemHeight() const;

    QAction* addMenu(QMenu* menu);
    ElaMenu* addMenu(const QString& title);
    ElaMenu* addMenu(const QIcon& icon, const QString& title);
    ElaMenu* addMenu(ElaIconType::IconName icon, const QString& title);

    QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text);
    QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text, const QKeySequence& shortcut);

    bool isHasChildMenu() const;
    bool isHasIcon() const;
Q_SIGNALS:
    Q_SIGNAL void menuShow();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAMENU_H
