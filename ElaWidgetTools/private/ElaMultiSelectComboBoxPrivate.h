#ifndef ELAMULTISELECTCOMBOBOXPRIVATE_H
#define ELAMULTISELECTCOMBOBOXPRIVATE_H
#include <QObject>
#include <QVector>

#include "ElaDef.h"
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
    ~ElaMultiSelectComboBoxPrivate() override;
    Q_SLOT void onItemPressed(const QModelIndex& index);

private:
    ElaComboBoxStyle* _comboBoxStyle{nullptr};
    ElaComboBoxView* _comboView{nullptr};
    QVector<bool> _itemSelection;
    bool _isFirstPopup{false};
    bool _isAllowHidePopup{false};
    QString _currentText;
    QStringList _selectedTextList;
    void _refreshCurrentIndexs();
    void _adjustSelectedVector();
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAMULTISELECTCOMBOBOXPRIVATE_H
