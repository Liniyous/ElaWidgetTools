#ifndef ELANAVIGATIONDELEGATE_H
#define ELANAVIGATIONDELEGATE_H

#include <QStyledItemDelegate>

#include "Def.h"
#include "stdafx.h"
class ElaNavigationView;
class ElaNavigationDelegatePrivate;
class ElaNavigationDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_Q_CREATE(ElaNavigationDelegate)
    Q_PROPERTY_CREATE_Q_H(qreal, Opacity);
    Q_PROPERTY_CREATE_Q_H(qreal, Rotate);
    Q_PRIVATE_CREATE_Q_H(ElaNavigationView*, NavigationView);

public:
    ElaNavigationDelegate(QObject* parent = nullptr);
    ~ElaNavigationDelegate();
    void navigationNodeStateChange(QVariantMap data);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // ELANAVIGATIONDELEGATE_H
