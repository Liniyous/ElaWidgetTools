#ifndef ELABREADCRUMBBARDELEGATE_H
#define ELABREADCRUMBBARDELEGATE_H

#include <QStyledItemDelegate>

#include "Def.h"
class ElaBreadcrumbBarDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ElaBreadcrumbBarDelegate(QObject* parent = nullptr);
    ~ElaBreadcrumbBarDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELABREADCRUMBBARDELEGATE_H
