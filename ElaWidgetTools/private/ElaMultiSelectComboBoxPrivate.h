#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMULTISELECTCOMBOBOXPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMULTISELECTCOMBOBOXPRIVATE_H_
#include "ElaPropertyMacro.h"
#include <QObject>
#include <QVector>

#include "ElaWidgetToolsDef.h"
class ElaComboBoxStyle;
class ElaComboBoxView;
class ElaMultiSelectComboBox;
class ElaMultiSelectComboBoxPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaMultiSelectComboBox)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY(qreal, ExpandIconRotate)
    ELA_PROPERTY(qreal, ExpandMarkWidth)
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

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMULTISELECTCOMBOBOXPRIVATE_H_

