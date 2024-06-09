#ifndef ELANAVIGATIONDELEGATE_H
#define ELANAVIGATIONDELEGATE_H

#include <QStyledItemDelegate>

#include "Def.h"
#include "stdafx.h"
class ElaEvent;
class QPropertyAnimation;
class ElaApplication;
class ElaNavigationNode;
class ElaNavigationView;
class ElaNavigationDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, Opacity);
    Q_PROPERTY_CREATE(qreal, Rotate);
    Q_PRIVATE_CREATE(ElaNavigationView*, NavigationView);

public:
    ElaNavigationDelegate(QObject* parent = nullptr);
    ~ElaNavigationDelegate();
    void navigationNodeStateChange(QMap<QString, QVariant> data);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
Q_SIGNALS:
private:
    qreal _lastSelectMarkTop{10};
    qreal _lastSelectMarkBottom{10};
    qreal _selectMarkTop{10};
    qreal _selectMarkBottom{10};
    bool _isSelectMarkDisplay{true};
    ElaNavigationNode* _opacityAnimationTargetNode{nullptr};
    ElaNavigationNode* _expandAnimationTargetNode{nullptr};
    ElaApplicationType::ThemeMode _themeMode;
    ElaNavigationNode* _lastSelectedNode{nullptr};
    bool _compareItemY(ElaNavigationNode* node1, ElaNavigationNode* node2);
    QPropertyAnimation* _lastSelectMarkTopAnimation{nullptr};
    QPropertyAnimation* _lastSelectMarkBottomAnimation{nullptr};
    QPropertyAnimation* _selectMarkTopAnimation{nullptr};
    QPropertyAnimation* _selectMarkBottomAnimation{nullptr};
};

#endif // ELANAVIGATIONDELEGATE_H
