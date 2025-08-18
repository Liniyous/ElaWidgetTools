#ifndef ELACALENDARPRIVATE_H
#define ELACALENDARPRIVATE_H

#include <QDate>
#include <QObject>
#include <QPixmap>

#include "ElaDef.h"
class ElaCalendar;
class ElaBaseListView;
class ElaCalendarModel;
class ElaCalendarDelegate;
class ElaToolButton;
class ElaCalendarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaCalendar)
    Q_PROPERTY_CREATE_D(int, BorderRaiuds)
    Q_PROPERTY_CREATE_D(QDate, SelectedDate)
    Q_PROPERTY_CREATE(qreal, ZoomRatio)
    Q_PROPERTY_CREATE(qreal, PixOpacity)
public:
    explicit ElaCalendarPrivate(QObject* parent = nullptr);
    ~ElaCalendarPrivate();
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

#endif // ELACALENDARPRIVATE_H
