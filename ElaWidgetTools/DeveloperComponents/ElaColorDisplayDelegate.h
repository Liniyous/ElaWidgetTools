#ifndef ELACOLORDISPLAYDELEGATE_H
#define ELACOLORDISPLAYDELEGATE_H

#include <QStyledItemDelegate>

#include "ElaDef.h"
class ElaColorDisplayDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_PRIVATE_CREATE(ElaThemeType::ThemeMode, ThemeMode)
public:
    explicit ElaColorDisplayDelegate(QObject* parent = nullptr);
    ~ElaColorDisplayDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // ELACOLORDISPLAYDELEGATE_H
