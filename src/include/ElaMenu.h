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
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaMenu(QWidget* parent = nullptr);
    explicit ElaMenu(const QString& title, QWidget* parent = nullptr);
    ~ElaMenu();
    QAction* addMenu(QMenu* menu);
    ElaMenu* addMenu(const QString& title);
    ElaMenu* addMenu(const QIcon& icon, const QString& title);
    ElaMenu* addMenu(ElaIconType icon, const QString& title);

    QAction* addElaIconAction(ElaIconType icon, const QString& text);
    QAction* addElaIconAction(ElaIconType icon, const QString& text, const QKeySequence& shortcut);

protected:
    virtual bool event(QEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;
    virtual void closeEvent(QCloseEvent* event) override;
};

#endif // ELAMENU_H
