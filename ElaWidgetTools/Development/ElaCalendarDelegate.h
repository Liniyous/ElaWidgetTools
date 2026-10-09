#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACALENDARDELEGATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACALENDARDELEGATE_H_

#include "ElaPropertyMacro.h"
#include <QDate>
#include <QStyledItemDelegate>

#include "ElaCalendarModel.h"
#include "ElaWidgetToolsDef.h"
class ElaCalendarDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    ELA_VARIABLE(int, ItemWidth)
    ELA_VARIABLE(int, ItemHeight)
    ELA_VARIABLE(bool, IsTransparent)
public:
    explicit ElaCalendarDelegate(ElaCalendarModel* calendarModel, QObject* parent = nullptr);
    ~ElaCalendarDelegate();

    Q_SLOT void onCalendarModelDisplayModeChanged();

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    ElaCalendarModel* _calendarModel{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    QDate _nowDate;
    void _drawYearOrMonth(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void _drawDays(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACALENDARDELEGATE_H_

