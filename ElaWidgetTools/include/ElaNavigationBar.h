#ifndef ELANAVIGATIONBAR_H
#define ELANAVIGATIONBAR_H

#include <QWidget>

#include "Def.h"
#include "stdafx.h"

class ElaNavigationBarPrivate;
class ELA_EXPORT ElaNavigationBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaNavigationBar)
    Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)
public:
    explicit ElaNavigationBar(QWidget* parent = nullptr);
    ~ElaNavigationBar() override;
    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(QString title);
    void setUserInfoCardSubTitle(QString subTitle);

    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None);

    bool getNavigationNodeIsExpanded(QString expanderKey) const;
    void expandNavigationNode(QString expanderKey);
    void collpaseNavigationNode(QString expanderKey);
    void removeNavigationNode(QString nodeKey);

    void setNodeKeyPoints(QString nodeKey, int keyPoints);
    int getNodeKeyPoints(QString nodeKey) const;

    void navigation(QString pageKey, bool isLogClicked = true);
    void setDisplayMode(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true);

Q_SIGNALS:
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void navigationNodeClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey);
    Q_SIGNAL void navigationNodeAdded(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);
    Q_SIGNAL void navigationNodeRemoved(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELANAVIGATIONBAR_H
