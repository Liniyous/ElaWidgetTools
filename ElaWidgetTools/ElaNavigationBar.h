#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELANAVIGATIONBAR_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELANAVIGATIONBAR_H_

#include <QWidget>

#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
#include "ElaSuggestBox.h"
class ElaNavigationBarPrivate;
class ELA_EXPORT ElaNavigationBar : public QWidget
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaNavigationBar)
    ELA_PROPERTY_Q_H(bool, IsTransparent)
    ELA_PROPERTY_Q_H(bool, IsAllowPageOpenInNewWindow)
public:
    explicit ElaNavigationBar(QWidget* parent = nullptr);
    ~ElaNavigationBar() override;

    void setNavigationBarWidth(int navigationBarWidth);
    Q_REQUIRED_RESULT int getNavigationBarWidth() const;

    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(const QPixmap& pix);
    void setUserInfoCardTitle(const QString& title);
    void setUserInfoCardSubTitle(const QString& subTitle);

    ElaNavigationType::NodeResult addExpanderNode(const QString& expanderTitle, QString& expanderKey, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, int keyPoints, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addFooterNode(const QString& footerTitle, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeResult addCategoryNode(const QString& categoryTitle, QString& categoryKey);
    ElaNavigationType::NodeResult addCategoryNode(const QString& categoryTitle, QString& categoryKey, const QString& targetExpanderKey);

    bool getNodeIsExpanded(const QString& expanderKey) const;
    void expandNode(const QString& expanderKey);
    void collapseNode(const QString& expanderKey);
    void removeNode(const QString& nodeKey);

    void setNodeKeyPoints(const QString& nodeKey, int keyPoints);
    int getNodeKeyPoints(const QString& nodeKey) const;

    void setNodeTitle(const QString& nodeKey, const QString& nodeTitle);
    QString getNodeTitle(const QString& nodeKey) const;

    void navigation(const QString& pageKey, bool isLogClicked = true, bool isRouteBack = false);
    void setDisplayMode(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true);
    ElaNavigationType::NavigationDisplayMode getDisplayMode() const;

    int getPageOpenInNewWindowCount(const QString& nodeKey) const;

    const QList<ElaSuggestBox::SuggestData>& getSuggestDataList() const;

Q_SIGNALS:
    Q_SIGNAL void pNavigationBarWidthChanged();
    Q_SIGNAL void pageOpenInNewWindow(QString nodeKey);
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void navigationNodeClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, bool isRouteBack);
    Q_SIGNAL void navigationNodeAdded(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);
    Q_SIGNAL void navigationNodeRemoved(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELANAVIGATIONBAR_H_
