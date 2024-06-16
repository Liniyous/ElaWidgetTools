#ifndef ELACOMBOBOXDELEGATE_H
#define ELACOMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

#include "Def.h"
#include "stdafx.h"
class ElaComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ElaComboBoxDelegate(QObject* parent = nullptr);
    ~ElaComboBoxDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);

private:
    QLinearGradient* _hovergradient{nullptr};
    QLinearGradient* _selectedgradient{nullptr};
    QLinearGradient* _selectedHovergradient{nullptr};
    ElaApplicationType::ThemeMode _themeMode;
};

#endif // ELACOMBOBOXDELEGATE_H
