#ifndef ELAMULTISELECTCOMBOBOXPRIVATE_H
#define ELAMULTISELECTCOMBOBOXPRIVATE_H
#include <QObject>

#include "Def.h"
#include "stdafx.h"
class ElaComboBoxStyle;
class ElaComboBoxView;
class ElaMultiSelectComboBox;
class ElaMultiSelectComboBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMultiSelectComboBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE(qreal, ExpandIconRotate)
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
public:
    explicit ElaMultiSelectComboBoxPrivate(QObject* parent = nullptr);
    ~ElaMultiSelectComboBoxPrivate();
    Q_SLOT void onItemPressed(const QModelIndex& index);

private:
    ElaComboBoxStyle* _comboBoxStyle{nullptr};
    bool _isAllowHidePopup{false};
    ElaComboBoxView* _comboView{nullptr};
    QVector<bool> _itemSelection;
    bool _isFirstPopup{false};
    QString _currentText;
    QStringList _selectedTextList;
    void _refreshCurrentIndexs();
    void _adjustSelectedVector();
    ElaApplicationType::ThemeMode _themeMode;
};

#endif // ELAMULTISELECTCOMBOBOXPRIVATE_H
