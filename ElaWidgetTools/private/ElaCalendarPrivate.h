#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACALENDARPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACALENDARPRIVATE_H_

#include "ElaCalendarModel.h"
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
class ElaScrollAnimation;
class QPropertyAnimation;
class QVariantAnimation;
class QTimer;
class ElaCalendarPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaCalendar)
    ELA_PROPERTY_D(int, BorderRaiuds)
    ELA_PROPERTY_D(QDate, SelectedDate)
public:
    explicit ElaCalendarPrivate(QObject* parent = nullptr);
    ~ElaCalendarPrivate() override;
    Q_SLOT void onSwitchButtonClicked();
    Q_SLOT void onCalendarViewClicked(const QModelIndex& index);
    Q_SLOT void onUpButtonClicked();
    Q_SLOT void onDownButtonClicked();
    Q_SLOT void onVisibleDateWindowTimeout();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QPixmap _oldCalendarViewPix;
    QPixmap _newCalendarViewPix;
    QDate _displayDate; // 当前视口中央的日期
    QDate _pageDate;    // 连续翻页的目标日期
    qreal _borderWidth{1};
    qreal _zoomRatio{1};
    qreal _pixOpacity{1};
    qreal _wheelRemainder{0}; // 尚未累计到整行目标的滚轮输入 单位为逻辑像素
    ElaThemeType::ThemeMode _themeMode;
    ElaBaseListView* _calendarView{nullptr};
    ElaCalendarModel* _calendarModel{nullptr};
    ElaCalendarDelegate* _calendarDelegate{nullptr};
    ElaBaseListView* _calendarTitleView{nullptr};
    ElaToolButton* _modeSwitchButton{nullptr};
    ElaToolButton* _upButton{nullptr};
    ElaToolButton* _downButton{nullptr};
    QTimer* _dateWindowTimer{nullptr};
    ElaScrollAnimation* _wheelAnimation{nullptr};
    QPropertyAnimation* _scrollAnimation{nullptr};
    QVariantAnimation* _switchAnimation{nullptr};
    bool _isSwitchAnimationFinished{true};
    bool _isDrawNewPix{false};
    bool _isZoomIn{false};
    bool _isUpdatingDateWindow{false};
    QDate _getPageStartDate(QDate date) const;
    void _scrollToDate(QDate date);
    void _scrollPage(int direction);
    void _switchDisplayMode(ElaCalendarType mode, QDate date);
    QPixmap _getContentPixmap(const QRect& contentRect) const;
    void _restoreSelection();
    void _updateNavigationIcons();
    void _updateSwitchButtonText();
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELACALENDARPRIVATE_H_
