#ifndef ELAMULTISELECTCOMBOBOX_H
#define ELAMULTISELECTCOMBOBOX_H
#include <QComboBox>

#include "ElaProperty.h"
class ElaMultiSelectComboBoxPrivate;
class ELA_EXPORT ElaMultiSelectComboBox : public QComboBox
{
    Q_OBJECT
    Q_Q_CREATE(ElaMultiSelectComboBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaMultiSelectComboBox(QWidget* parent = nullptr);
    ~ElaMultiSelectComboBox();
    void setCurrentSelection(QString selection);
    void setCurrentSelection(QStringList selection);
    void setCurrentSelection(int index);
    void setCurrentSelection(QList<int> selectionIndex);
    QStringList getCurrentSelection() const;
    QList<int> getCurrentSelectionIndex() const;
Q_SIGNALS:
    Q_SIGNAL void itemSelectionChanged(QVector<bool> itemSelection);
    Q_SIGNAL void currentTextListChanged(QStringList selectedTextList);

protected:
    void paintEvent(QPaintEvent* e) override;
    void showPopup() override;
    void hidePopup() override;
};

#endif // ELAMULTISELECTCOMBOBOX_H
