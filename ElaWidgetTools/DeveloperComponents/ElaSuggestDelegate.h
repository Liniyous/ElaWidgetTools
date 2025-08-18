#ifndef ELASUGGESTDELEGATE_H
#define ELASUGGESTDELEGATE_H

#include <QStyledItemDelegate>

#include "ElaDef.h"
class ElaSuggestDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ElaSuggestDelegate(QObject* parent = nullptr);
    ~ElaSuggestDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELASUGGESTDELEGATE_H
