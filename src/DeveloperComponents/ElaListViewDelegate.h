#ifndef ELALISTVIEWDELEGATE_H
#define ELALISTVIEWDELEGATE_H

#include <QStyledItemDelegate>

#include "Def.h"
class ElaListViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ElaListViewDelegate(QObject* parent = nullptr);
    ~ElaListViewDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELALISTVIEWDELEGATE_H
