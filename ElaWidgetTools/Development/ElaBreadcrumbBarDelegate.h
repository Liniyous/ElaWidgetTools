#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELABREADCRUMBBARDELEGATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELABREADCRUMBBARDELEGATE_H_

#include "ElaPropertyMacro.h"
#include <QModelIndex>
#include <QStyledItemDelegate>

#include "ElaWidgetToolsDef.h"
class ElaBreadcrumbBarDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    ELA_VARIABLE(QModelIndex, PressIndex)
public:
    explicit ElaBreadcrumbBarDelegate(QObject* parent = nullptr);
    ~ElaBreadcrumbBarDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELABREADCRUMBBARDELEGATE_H_

