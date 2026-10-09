#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELACHECKBOX_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELACHECKBOX_H_

#include <QCheckBox>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"
class ELA_EXPORT ElaCheckBox : public QCheckBox
{
    Q_OBJECT
    ELA_PROPERTY(int, BorderRadius)
public:
    explicit ElaCheckBox(QWidget* parent = nullptr);
    explicit ElaCheckBox(const QString& text, QWidget* parent = nullptr);
    ~ElaCheckBox() override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELACHECKBOX_H_
