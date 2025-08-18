#ifndef ELACALENDARTITLEDELEGATE_H
#define ELACALENDARTITLEDELEGATE_H

#include <QStyledItemDelegate>

#include "ElaDef.h"
class ElaCalendarTitleDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ElaCalendarTitleDelegate(QObject* parent = nullptr);
    ~ElaCalendarTitleDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELACALENDARTITLEDELEGATE_H
