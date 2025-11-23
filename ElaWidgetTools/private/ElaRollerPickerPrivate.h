#ifndef ELAFRAMEWORK_ELAROLLERPICKERPRIVATE_H
#define ELAFRAMEWORK_ELAROLLERPICKERPRIVATE_H
#include "ElaDef.h"
#include "ElaRoller.h"
#include "ElaRollerPickerContainer.h"
#include <QObject>
class QHBoxLayout;
class ElaRollerPicker;
class ElaRollerPickerPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaRollerPicker)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
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

#endif //ELAFRAMEWORK_ELAROLLERPICKERPRIVATE_H
