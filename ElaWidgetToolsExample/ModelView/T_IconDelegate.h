#ifndef ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_ICONDELEGATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_ICONDELEGATE_H_

#include <QStyledItemDelegate>

#include "ElaWidgetToolsDef.h"
class T_IconDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit T_IconDelegate(QObject* parent = nullptr);
    ~T_IconDelegate() override;

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLSEXAMPLE_MODELVIEW_T_ICONDELEGATE_H_


