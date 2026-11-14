#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACALENDARPICKERPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACALENDARPICKERPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>
#include <QPoint>

#include "ElaWidgetToolsDef.h"
class ElaCalendar;
class ElaCalendarPicker;
class ElaCalendarPickerContainer;
class ElaControlAnimation;
class QTimer;
class QVariantAnimation;
class ElaCalendarPickerPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaCalendarPicker)
    ELA_PROPERTY_D(int, BorderRadius)
public:
    explicit ElaCalendarPickerPrivate(QObject* parent = nullptr);
    ~ElaCalendarPickerPrivate() override;
    Q_SLOT void onCalendarPickerClicked();
    Q_SLOT void onPopupDelayTimeout();
    Q_SLOT void onCalendarSelectedDateChanged();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaCalendar* _calendar{nullptr};
    ElaCalendarPickerContainer* _calendarPickerContainer{nullptr};
    ElaControlAnimation* _controlAnimation{nullptr};
    QTimer* _popupDelayTimer{nullptr};
    QVariantAnimation* _popupAnimation{nullptr};
    QPoint _popupPosition;
    int _popupOffset{0};
    qreal _popupStartOpacity{1};
    bool _isClosing{false};
    void _closePopup();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACALENDARPICKERPRIVATE_H_

