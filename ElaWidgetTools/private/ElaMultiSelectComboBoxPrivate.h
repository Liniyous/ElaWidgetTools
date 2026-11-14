#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMULTISELECTCOMBOBOXPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMULTISELECTCOMBOBOXPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"

#include <QObject>
#include <QPersistentModelIndex>
#include <QRect>
#include <QStringList>
#include <QVector>

class ElaComboBoxStyle;
class ElaComboBoxView;
class ElaComboBoxPopup;
class ElaControlAnimation;
class ElaMultiSelectComboBox;
class ElaMultiSelectComboBoxPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaMultiSelectComboBox)
    ELA_PROPERTY_D(int, BorderRadius)
public:
    explicit ElaMultiSelectComboBoxPrivate(QObject* parent = nullptr);
    Q_SLOT void onItemPressed(const QModelIndex& index);
    Q_SLOT void onModelChanged();
    Q_SLOT void onCurrentIndexChanged(int index);

private:
    struct SelectionTag {
        QPersistentModelIndex index; // 标签对应的模型选项 溢出计数标签为空
        QRect rect;                  // 标签在组合框内的逻辑像素区域
        QRect closeRect;             // 取消选择按钮的逻辑像素区域
        QString text;                // 单行省略后的标签文字
        QString toolTip;             // 完整选项名称或隐藏选项名称
        int hiddenCount{0};          // 溢出标签代表的隐藏项数量
    };
    ElaComboBoxStyle* _comboBoxStyle{nullptr};
    ElaComboBoxView* _comboView{nullptr};
    ElaComboBoxPopup* _popup{nullptr};
    ElaControlAnimation* _controlAnimation{nullptr};
    QVector<QMetaObject::Connection> _modelConnections;
    QVector<bool> _lastSelection;
    QStringList _selectedTextList;
    QPersistentModelIndex _pressedTagIndex;
    bool _isUpdatingSelection{false};
    bool _isSelectionInitialized{false};
    bool _isPopupPressed{false};
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
    void _setSelection(const QList<int>& rows);
    void _refreshSelection();
    QModelIndexList _getSelectedIndexes() const;
    QVector<SelectionTag> _getSelectionTags() const;
    static QString _getSingleLineText(QString text);
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAMULTISELECTCOMBOBOXPRIVATE_H_
