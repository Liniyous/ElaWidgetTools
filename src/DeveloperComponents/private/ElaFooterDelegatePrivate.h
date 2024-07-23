#ifndef ELAFOOTERDELEGATEPRIVATE_H
#define ELAFOOTERDELEGATEPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"
class QLinearGradient;
class ElaNavigationNode;
class QPropertyAnimation;
class ElaListView;
class ElaFooterDelegate;
class ElaFooterDelegatePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaFooterDelegate)
    Q_PRIVATE_CREATE_D(ElaListView*, ElaListView);

public:
    explicit ElaFooterDelegatePrivate(QObject* parent = nullptr);
    ~ElaFooterDelegatePrivate();

private:
    qreal _lastSelectMarkTop{10};
    qreal _lastSelectMarkBottom{10};
    qreal _selectMarkTop{10};
    qreal _selectMarkBottom{10};
    bool _isSelectMarkDisplay{true};
    ElaNavigationNode* _lastSelectedNode{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    QPropertyAnimation* _lastSelectMarkTopAnimation{nullptr};
    QPropertyAnimation* _lastSelectMarkBottomAnimation{nullptr};
    QPropertyAnimation* _selectMarkTopAnimation{nullptr};
    QPropertyAnimation* _selectMarkBottomAnimation{nullptr};
    bool _compareItemY(ElaNavigationNode* node1, ElaNavigationNode* node2);
};

#endif // ELAFOOTERDELEGATEPRIVATE_H
