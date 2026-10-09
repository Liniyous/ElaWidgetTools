#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELALINEEDIT_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELALINEEDIT_H_

#include <QLineEdit>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaLineEditPrivate;
class ELA_EXPORT ElaLineEdit : public QLineEdit
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaLineEdit)
    ELA_PROPERTY_Q_H(int, BorderRadius)
public:
    explicit ElaLineEdit(QWidget* parent = nullptr);
    ~ElaLineEdit() override;

    void setIsClearButtonEnable(bool isClearButtonEnable);
    Q_REQUIRED_RESULT bool getIsClearButtonEnable() const;

Q_SIGNALS:
    Q_SIGNAL void pIsClearButtonEnableChanged();
    Q_SIGNAL void focusIn(QString text);
    Q_SIGNAL void focusOut(QString text);
    Q_SIGNAL void wmFocusOut(QString text);

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELALINEEDIT_H_
