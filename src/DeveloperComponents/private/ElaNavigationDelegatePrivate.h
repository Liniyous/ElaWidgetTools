#ifndef ELANAVIGATIONDELEGATEPRIVATE_H
#define ELANAVIGATIONDELEGATEPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"

class QPropertyAnimation;
class ElaApplication;
class ElaNavigationNode;
class ElaNavigationView;
class ElaNavigationDelegate;
class ElaNavigationDelegatePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaNavigationDelegate)
    Q_PROPERTY_CREATE_D(qreal, Opacity);
    Q_PROPERTY_CREATE_D(qreal, Rotate);
    Q_PRIVATE_CREATE_D(ElaNavigationView*, NavigationView);

public:
    explicit ElaNavigationDelegatePrivate(QObject* parent = nullptr);
    ~ElaNavigationDelegatePrivate();

private:
    qreal _lastSelectMarkTop{10};
    qreal _lastSelectMarkBottom{10};
    qreal _selectMarkTop{10};
    qreal _selectMarkBottom{10};
    bool _isSelectMarkDisplay{true};
    ElaNavigationNode* _opacityAnimationTargetNode{nullptr};
    ElaNavigationNode* _expandAnimationTargetNode{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    ElaNavigationNode* _lastSelectedNode{nullptr};
    bool _compareItemY(ElaNavigationNode* node1, ElaNavigationNode* node2);
    QPropertyAnimation* _lastSelectMarkTopAnimation{nullptr};
    QPropertyAnimation* _lastSelectMarkBottomAnimation{nullptr};
    QPropertyAnimation* _selectMarkTopAnimation{nullptr};
    QPropertyAnimation* _selectMarkBottomAnimation{nullptr};
};

#endif // ELANAVIGATIONDELEGATEPRIVATE_H
