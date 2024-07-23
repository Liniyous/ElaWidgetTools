#ifndef T_ICONDELEGATE_H
#define T_ICONDELEGATE_H

#include <QStyledItemDelegate>

#include "Def.h"
class T_IconDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit T_IconDelegate(QObject* parent = nullptr);
    ~T_IconDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
};

#endif // T_ICONDELEGATE_H
