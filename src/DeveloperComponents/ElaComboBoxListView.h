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
};

#endif // ELACOMBOBOXLISTVIEW_H
