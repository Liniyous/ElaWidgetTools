#ifndef ELACOMBOBOXLISTVIEW_H
#define ELACOMBOBOXLISTVIEW_H

#include <QListView>

#include "Def.h"
class ElaComboBoxListView : public QListView
{
    Q_OBJECT
public:
    explicit ElaComboBoxListView(QWidget* parent = nullptr);
    ~ElaComboBoxListView();
    Q_SLOT void onThemeChanged(ElaApplicationType::ThemeMode themeMode);
Q_SIGNALS:
    Q_SIGNAL void itemPressed(const QModelIndex& index);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // ELACOMBOBOXLISTVIEW_H
