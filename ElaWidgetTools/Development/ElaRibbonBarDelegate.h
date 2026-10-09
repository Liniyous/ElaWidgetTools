#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELARIBBONBARDELEGATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELARIBBONBARDELEGATE_H_

#include "ElaPropertyMacro.h"
#include <QModelIndex>
#include <QStyledItemDelegate>

#include "ElaWidgetToolsDef.h"
class ElaRibbonBarDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    ELA_VARIABLE(QModelIndex, PressIndex);
    ELA_VARIABLE(int, BottomMargin);

public:
    explicit ElaRibbonBarDelegate(QObject* parent = nullptr);
    ~ElaRibbonBarDelegate() override;

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELARIBBONBARDELEGATE_H_

