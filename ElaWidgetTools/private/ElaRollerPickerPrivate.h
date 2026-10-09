#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAROLLERPICKERPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAROLLERPICKERPRIVATE_H_
#include "ElaPropertyMacro.h"
#include "ElaRoller.h"
#include "ElaRollerPickerContainer.h"
#include "ElaWidgetToolsDef.h"
#include <QObject>
class QHBoxLayout;
class ElaRollerPicker;
class ElaRollerPickerPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaRollerPicker)
    ELA_PROPERTY_D(int, BorderRadius)
public:
    explicit ElaRollerPickerPrivate(QObject* parent = nullptr);
    ~ElaRollerPickerPrivate() override;
    Q_SLOT void onRollerPickerClicked();
    Q_SLOT void onOverButtonClicked();
    Q_SLOT void onCancelButtonClicked();

private:
    int _pickerRollerHeight{245};
    ElaThemeType::ThemeMode _themeMode;
    QHBoxLayout* _containerLayout{nullptr};
    ElaRollerPickerContainer* _rollerPickerContainer{nullptr};
    int _getRollerTotalWidth() const;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAROLLERPICKERPRIVATE_H_

