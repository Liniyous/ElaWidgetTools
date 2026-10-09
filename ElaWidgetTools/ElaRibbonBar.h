#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELARIBBONBAR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELARIBBONBAR_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"

#include <QTabBar>
#include <QWidget>

class ElaPivot;
class ElaRibbonBarPrivate;
class ELA_EXPORT ElaRibbonBar : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaRibbonBar)
public:
    explicit ElaRibbonBar(QWidget* parent = nullptr);
    ~ElaRibbonBar() override;

    // 设置绑定的TabBar 切换Ribbon页时进行同步 可以不设置
    void setRibbonTabBar(QTabBar* tabBar);
    QTabBar* getRibbonTabBar() const;

    // 设置绑定的Pivot 切换Ribbon页时进行同步 可以不设置
    void setRibbonPivot(ElaPivot* pivot);
    ElaPivot* getRibbonPivot() const;

    void switchRibbonPage(const QString& pageName);
    const QString& getCurrentRibbonPage() const;

    void setRibbonPageEnable(const QString& pageName, bool isEnable);
    bool getRibbonPageEnable(const QString& pageName) const;

    // 添加到指定Ribbon页的根组
    QAction* addRibbonAction(const QString& pageName, const QString& actionName);
    // 添加到指定Ribbon页的根组 并指定图标（随主题变化）
    QAction* addRibbonAction(const QString& pageName, const QString& actionName, ElaIconType::IconName icon);
    // 添加到指定Ribbon页的根组 并指定QIcon
    QAction* addRibbonAction(const QString& pageName, const QString& actionName, const QIcon& icon);
    // 添加到指定Ribbon页的指定组
    QAction* addRibbonAction(const QString& pageName, const QString& groupName, const QString& actionName);
    // 添加到指定Ribbon页的指定组 并指定图标（随主题变化）
    QAction* addRibbonAction(const QString& pageName, const QString& groupName, const QString& actionName, ElaIconType::IconName icon);
    // 添加到指定Ribbon页的指定组 并指定QIcon
    QAction* addRibbonAction(const QString& pageName, const QString& groupName, const QString& actionName, const QIcon& icon);
    // 添加自定义窗口到指定Ribbon页的指定组
    QAction* addRibbonWidget(const QString& pageName, const QString& groupName, QWidget* widget);

    void removeRibbonAction(QAction* action);
    void removeRibbonAction(const QString& pageName, const QString& groupName, const QString& actionName);

    QList<QAction*> getRibbonActions(const QString& pageName);
    QList<QAction*> getRibbonActions(const QString& pageName, const QString& groupName);
    QList<QAction*> getRibbonActions(const QString& pageName, const QString& groupName, const QString& actionName);

    int getGroupBottomMargin() const;
Q_SIGNALS:
    Q_SIGNAL void ribbonPageChanged(const QString& pageName);
    Q_SIGNAL void ribbonActionTriggered(const QString& pageName, const QString& groupName, QAction* action);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELARIBBONBAR_H_
