#ifndef ELANAVIGATIONBAR_H
#define ELANAVIGATIONBAR_H

#include <QWidget>

#include "Def.h"
#include "stdafx.h"

class ElaNavigationBarPrivate;
class ElaNavigationBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaNavigationBar)
public:
    explicit ElaNavigationBar(QWidget* parent = nullptr);
    ~ElaNavigationBar();
    void setUserInfoCardVisible(bool isVisible);
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, ElaIconType awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, ElaIconType = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, ElaIconType awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, ElaIconType awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, int keyPoints = 0, ElaIconType awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints = 0, ElaIconType awesome = ElaIconType::None) const;
    ElaNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, ElaIconType awesome = ElaIconType::None) const;
    void navigation(QString pageKey, bool isLogClicked = true);
    void switchCompact(bool direction);
Q_SIGNALS:
    Q_SIGNAL void footerNodeClicked(QString footerKey); // 页脚没有绑定页面时发送
protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    friend class ElaWindow;
    friend class ElaWindowPrivate;
};

#endif // ELANAVIGATIONBAR_H
