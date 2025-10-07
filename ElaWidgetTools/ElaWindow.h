#ifndef ELAWINDOW_H
#define ELAWINDOW_H

#include <QMainWindow>

#include "ElaAppBar.h"
#include "ElaDef.h"

class ElaWindowPrivate;
class ELA_EXPORT ElaWindow : public QMainWindow
{
    Q_OBJECT
    Q_Q_CREATE(ElaWindow)
    Q_PROPERTY_CREATE_Q_H(bool, IsStayTop)
    Q_PROPERTY_CREATE_Q_H(bool, IsFixedSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsDefaultClosed)
    Q_PROPERTY_CREATE_Q_H(int, AppBarHeight)
    Q_PROPERTY_CREATE_Q_H(int, CustomWidgetMaximumWidth)
    Q_PROPERTY_CREATE_Q_H(int, ThemeChangeTime)
    Q_PROPERTY_CREATE_Q_H(bool, IsCentralStackedWidgetTransparent)
    Q_PROPERTY_CREATE_Q_H(bool, IsAllowPageOpenInNewWindow)
    Q_PROPERTY_CREATE_Q_H(bool, IsNavigationBarEnable)
    Q_PROPERTY_CREATE_Q_H(int, NavigationBarWidth)
    Q_PROPERTY_CREATE_Q_H(int, CurrentStackIndex)
    Q_PROPERTY_CREATE_Q_H(ElaNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
    Q_PROPERTY_CREATE_Q_H(ElaWindowType::StackSwitchMode, StackSwitchMode)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    Q_INVOKABLE explicit ElaWindow(QWidget* parent = nullptr);
    ~ElaWindow() override;

    void moveToCenter();

    void setCustomWidget(ElaAppBarType::CustomArea customArea, QWidget* customWidget);
    QWidget* getCustomWidget() const;

    void setCentralCustomWidget(QWidget* customWidget);
    QWidget* getCentralCustomWidget() const;

    void setCustomMenu(QMenu* customMenu);
    QMenu* getCustomMenu() const;

    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(QString title);
    void setUserInfoCardSubTitle(QString subTitle);
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None);
    ElaNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None);

    void addCentralWidget(QWidget* centralWidget);
    QWidget* getCentralWidget(int index) const;

    bool getNavigationNodeIsExpanded(QString expanderKey) const;
    void expandNavigationNode(QString expanderKey);
    void collapseNavigationNode(QString expanderKey);
    void removeNavigationNode(QString nodeKey) const;
    int getPageOpenInNewWindowCount(QString nodeKey) const;
    void backtrackNavigationNode(QString nodeKey);

    void setNodeKeyPoints(QString nodeKey, int keyPoints);
    int getNodeKeyPoints(QString nodeKey) const;

    void setNavigationNodeTitle(QString nodeKey, QString nodeTitle);
    QString getNavigationNodeTitle(QString nodeKey) const;

    void navigation(QString pageKey);
    int getCurrentNavigationIndex() const;
    QString getCurrentNavigationPageKey() const;
    void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags);
    ElaAppBarType::ButtonFlags getWindowButtonFlags() const;

    void closeWindow();
Q_SIGNALS:
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void closeButtonClicked();
    Q_SIGNAL void navigationNodeClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey);
    Q_SIGNAL void customWidgetChanged();
    Q_SIGNAL void centralCustomWidgetChanged();
    Q_SIGNAL void customMenuChanged();
    Q_SIGNAL void pageOpenInNewWindow(QString nodeKey);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual QMenu* createPopupMenu() override;
};

#endif // ELAWINDOW_H
