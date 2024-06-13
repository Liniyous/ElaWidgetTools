#ifndef ELAMENU_H
#define ELAMENU_H

#include <QMenu>
#include <QWidget>

#include "Def.h"
#include "stdafx.h"
class ElaMenuPrivate;
class ELA_EXPORT ElaMenu : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaMenu)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaMenu(QWidget* parent = nullptr);
    ~ElaMenu();
    void setMenuItemHeight(int height);
    void popup(const QPoint& pos, QAction* at = nullptr);
    ElaMenu* addMenu(const QString& text);
    ElaMenu* addMenu(ElaIconType icon, const QString& text);
    QAction* addMenu(ElaMenu* menu);
    QAction* addAction(const QString& text);
    QAction* addAction(ElaIconType icon, const QString& text);
    QAction* addAction(const QString& text, const QKeySequence& shortcut);
    QAction* addAction(ElaIconType icon, const QString& text, const QKeySequence& shortcut);

protected:
    virtual bool event(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void closeEvent(QCloseEvent* event) override;
};

#endif // ELAMENU_H
