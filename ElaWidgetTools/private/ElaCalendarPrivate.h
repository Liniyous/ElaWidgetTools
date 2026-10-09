#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACALENDARPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACALENDARPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QDate>
#include <QObject>
#include <QPixmap>

#include "ElaWidgetToolsDef.h"
class ElaCalendar;
class ElaBaseListView;
class ElaCalendarModel;
class ElaCalendarDelegate;
class ElaToolButton;
class ElaCalendarPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaCalendar)
    ELA_PROPERTY_D(int, BorderRaiuds)
    ELA_PROPERTY_D(QDate, SelectedDate)
    ELA_PROPERTY(qreal, ZoomRatio)
    ELA_PROPERTY(qreal, PixOpacity)
public:
    explicit ElaCalendarPrivate(QObject* parent = nullptr);
    ~ElaCalendarPrivate() override;
    Q_SLOT void onSwitchButtonClicked();
    Q_SLOT void onCalendarViewClicked(const QModelIndex& index);
    Q_SLOT void onUpButtonClicked();
    Q_SLOT void onDownButtonClicked();

private:
    QPixmap _oldCalendarViewPix;
    QPixmap _newCalendarViewPix;
    int _lastSelectedYear{0};
    int _lastSelectedMonth{1};
    qreal _borderWidth{1.5};
    ElaThemeType::ThemeMode _themeMode;
    ElaBaseListView* _calendarView{nullptr};
    ElaCalendarModel* _calendarModel{nullptr};
    ElaCalendarDelegate* _calendarDelegate{nullptr};
    ElaBaseListView* _calendarTitleView{nullptr};
    ElaToolButton* _modeSwitchButton{nullptr};
    ElaToolButton* _upButton{nullptr};
    ElaToolButton* _downButton{nullptr};
    bool _isSwitchAnimationFinished{true};
    bool _isDrawNewPix{false};
    void _scrollToDate(QDate date);
    void _doSwitchAnimation(bool isZoomIn);
    void _updateSwitchButtonText();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACALENDARPRIVATE_H_

