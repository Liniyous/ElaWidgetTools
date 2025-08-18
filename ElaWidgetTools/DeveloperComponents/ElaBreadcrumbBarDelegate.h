#ifndef ELABREADCRUMBBARDELEGATE_H
#define ELABREADCRUMBBARDELEGATE_H

#include <QModelIndex>
#include <QStyledItemDelegate>

#include "ElaDef.h"
class ElaBreadcrumbBarDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_PRIVATE_CREATE(QModelIndex, PressIndex)
public:
    explicit ElaBreadcrumbBarDelegate(QObject* parent = nullptr);
    ~ElaBreadcrumbBarDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELABREADCRUMBBARDELEGATE_H
