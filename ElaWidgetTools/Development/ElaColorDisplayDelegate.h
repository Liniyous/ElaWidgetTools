#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOLORDISPLAYDELEGATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOLORDISPLAYDELEGATE_H_

#include "ElaPropertyMacro.h"
#include <QStyledItemDelegate>

#include "ElaWidgetToolsDef.h"
class ElaColorDisplayDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    ELA_VARIABLE(ElaThemeType::ThemeMode, ThemeMode)
public:
    explicit ElaColorDisplayDelegate(QObject* parent = nullptr);
    ~ElaColorDisplayDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOLORDISPLAYDELEGATE_H_

