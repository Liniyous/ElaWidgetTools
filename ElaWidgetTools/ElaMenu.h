#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAMENU_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAMENU_H_

#include <QMenu>
#include <QWidget>

#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaMenuPrivate;
class ELA_EXPORT ElaMenu : public QMenu
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaMenu)

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

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAMENU_H_
