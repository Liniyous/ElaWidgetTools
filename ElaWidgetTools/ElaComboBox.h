#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELACOMBOBOX_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELACOMBOBOX_H_

#include <QComboBox>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaComboBoxPrivate;
class ELA_EXPORT ElaComboBox : public QComboBox
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaComboBox);
    ELA_PROPERTY_Q_H(int, BorderRadius)
public:
    explicit ElaComboBox(QWidget* parent = nullptr);
    ~ElaComboBox() override;

    void setEditable(bool editable);

protected:
    virtual void showPopup() override;
    virtual void hidePopup() override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELACOMBOBOX_H_
