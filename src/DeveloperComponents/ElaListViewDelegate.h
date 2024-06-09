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
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QLinearGradient* _hovergradient{nullptr};
    QLinearGradient* _selectedgradient{nullptr};
    QLinearGradient* _selectedHovergradient{nullptr};
    ElaApplicationType::ThemeMode _themeMode;
};

#endif // ELALISTVIEWDELEGATE_H
