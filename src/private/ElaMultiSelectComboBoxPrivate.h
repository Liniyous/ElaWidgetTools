#ifndef ELAMULTISELECTCOMBOBOXPRIVATE_H
#define ELAMULTISELECTCOMBOBOXPRIVATE_H
#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaComboBoxView;
class ElaMultiSelectComboBox;
class ElaMultiSelectComboBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMultiSelectComboBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit ElaMultiSelectComboBoxPrivate(QObject* parent = nullptr);
    ~ElaMultiSelectComboBoxPrivate();
    Q_SLOT void onItemPressed(const QModelIndex& index);

private:
    ElaComboBoxView* _comboView{nullptr};
    QVector<bool> _itemSelection;
    bool _isFirstPopup{false};
    QString _currentText;
    QStringList _selectedTextList;
    bool _isHidePopupAnimationFinished{true};
    void _refreshCurrentIndexs();
    void _adjustSelectedVector();
    ElaApplicationType::ThemeMode _themeMode;
};

#endif // ELAMULTISELECTCOMBOBOXPRIVATE_H
