#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELAMULTISELECTCOMBOBOX_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELAMULTISELECTCOMBOBOX_H_
#include <QComboBox>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ElaMultiSelectComboBoxPrivate;
class ELA_EXPORT ElaMultiSelectComboBox : public QComboBox
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaMultiSelectComboBox)
    ELA_PROPERTY_Q_H(int, BorderRadius)
public:
    explicit ElaMultiSelectComboBox(QWidget* parent = nullptr);
    ~ElaMultiSelectComboBox();
    void setCurrentSelection(const QString& selection);
    void setCurrentSelection(const QStringList& selection);
    void setCurrentSelection(int index);
    void setCurrentSelection(const QList<int>& selectionIndex);
    const QStringList& getCurrentSelection() const;
    QList<int> getCurrentSelectionIndex() const;
Q_SIGNALS:
    Q_SIGNAL void itemSelectionChanged(QVector<bool> itemSelection);
    Q_SIGNAL void currentTextListChanged(const QStringList& selectedTextList);

protected:
    void paintEvent(QPaintEvent* e) override;
    void showPopup() override;
    void hidePopup() override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELAMULTISELECTCOMBOBOX_H_
