#ifndef ELAWINDOW_H
#define ELAWINDOW_H

#include <QMainWindow>

#include "Def.h"
#include "stdafx.h"
class ElaWindowPrivate;
class QStackedWidget;
class ELA_EXPORT ElaWindow : public QMainWindow
{
    Q_OBJECT
    Q_Q_CREATE(ElaWindow)
    Q_PROPERTY_CREATE_Q_H(bool, IsStayTop)
    Q_PROPERTY_CREATE_Q_H(bool, IsFixedSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsDefaultClosed)
    Q_PROPERTY_CREATE_Q_H(int, AppBarHeight)
    Q_PROPERTY_CREATE_Q_H(QWidget*, CustomWidget)
    Q_PROPERTY_CREATE_Q_H(int, CustomWidgetMaximumWidth)
    Q_PROPERTY_CREATE_Q_H(int, ThemeChangeTime)
    Q_PROPERTY_CREATE_Q_H(bool, IsCentralStackedWidgetTransparent)
    Q_PROPERTY_CREATE_Q_H(bool, IsEnableMica)
    Q_PROPERTY_CREATE_Q_H(QString, MicaImagePath)
    Q_PROPERTY_CREATE_Q_H(ElaNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
public:
    explicit ElaWindow(QWidget* parent = nullptr);
    ~ElaWindow();

    void moveToCenter();

    void setIsNavigationBarEnable(bool isEnable);
    bool getIsNavigationBarEnable() const;
    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(QString title);
    void setUserInfoCardSubTitle(QString subTitle);
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None) const;

    void setNodeKeyPoints(QString nodeKey, int keyPoints);
    int getNodeKeyPoints(QString nodeKey) const;

    void navigation(QString pageKey);
    void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags);
    ElaAppBarType::ButtonFlags getWindowButtonFlags() const;

    void closeWindow();
Q_SIGNALS:
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void closeButtonClicked();
    Q_SIGNAL void navigationNodeClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey);

protected:
    virtual void moveEvent(QMoveEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual QMenu* createPopupMenu() override;
};

#endif // ELAWINDOW_H
