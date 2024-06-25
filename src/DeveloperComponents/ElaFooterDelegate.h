#ifndef ELAFOOTERDELEGATE_H
#define ELAFOOTERDELEGATE_H

#include <QStyledItemDelegate>

#include "stdafx.h"
class ElaNavigationNode;
class QPropertyAnimation;
class ElaListView;
class ElaFooterDelegatePrivate;
class ElaFooterDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_Q_CREATE(ElaFooterDelegate)
    Q_PRIVATE_CREATE_Q_H(ElaListView*, ElaListView);

public:
    explicit ElaFooterDelegate(QObject* parent = nullptr);
    ~ElaFooterDelegate();
    void navigationNodeStateChange(QVariantMap data);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // ELAFOOTERDELEGATE_H
