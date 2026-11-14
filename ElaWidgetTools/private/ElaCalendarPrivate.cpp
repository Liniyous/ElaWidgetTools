#include "ElaCalendarPrivate.h"

#include "ElaBaseListView.h"
#include "ElaCalendar.h"
#include "ElaCalendarDelegate.h"
#include "ElaColorAnimation.h"
#include "ElaScrollAnimation.h"
#include "ElaScrollBar.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"

#include <QApplication>
#include <QLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QScopedValueRollback>
#include <QScrollBar>
#include <QTimer>
#include <QVariantAnimation>
#include <QWheelEvent>

ElaCalendarPrivate::ElaCalendarPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaCalendarPrivate::~ElaCalendarPrivate()
{
}

void ElaCalendarPrivate::onSwitchButtonClicked()
{
    const auto mode = _calendarModel->getDisplayMode();
    if (mode != YearMode)
    {
        _switchDisplayMode(mode == DayMode ? MonthMode : YearMode, _displayDate);
    }
}

void ElaCalendarPrivate::onCalendarViewClicked(const QModelIndex& index)
{
    Q_Q(ElaCalendar);
    if (!_isSwitchAnimationFinished)
    {
        return;
    }
    const QDate date = _calendarModel->getDateFromIndex(index);
    if (!date.isValid())
    {
        return;
    }
    switch (_calendarModel->getDisplayMode())
    {
    case YearMode:
    {
        _switchDisplayMode(MonthMode, QDate(date.year(), _displayDate.month(), 1));
        break;
    }
    case MonthMode:
    {
        _switchDisplayMode(DayMode, date);
        break;
    }
    case DayMode:
    {
        q->setSelectedDate(date);
        Q_EMIT q->clicked(_pSelectedDate);
        break;
    }
    }
}

void ElaCalendarPrivate::onUpButtonClicked()
{
    _scrollPage(-1);
}

void ElaCalendarPrivate::onDownButtonClicked()
{
    _scrollPage(1);
}

void ElaCalendarPrivate::onVisibleDateWindowTimeout()
{
    if (!_isSwitchAnimationFinished || _scrollAnimation->state() == QAbstractAnimation::Running)
    {
        return;
    }
    auto* scrollBar = static_cast<ElaScrollBar*>(_calendarView->verticalScrollBar());
    const bool isScrolling = _wheelAnimation->state() == QAbstractAnimation::Running;
    const int remaining = isScrolling ? qRound(_wheelAnimation->getTargetPosition().x()) - scrollBar->value() : 0;
    auto index = _calendarView->indexAt(QPoint(5, 0));
    const QDate date = _calendarModel->getDateFromIndex(index);
    const int offset = _calendarView->visualRect(index).top();
    const QScopedValueRollback<bool> updating(_isUpdatingDateWindow, true);
    if (_calendarModel->ensureDateVisible(date))
    {
        _calendarView->doItemsLayout();
        _calendarView->scrollTo(_calendarModel->getIndexFromDate(date), QAbstractItemView::PositionAtTop);
        scrollBar->setValue(scrollBar->value() - offset);
        _restoreSelection();
        _updateSwitchButtonText();
        if (isScrolling)
        {
            // 换窗后接续剩余惯性 保持当前像素位置与运动方向
            scrollBar->scrollTo(scrollBar->value() + remaining);
        }
    }
}

bool ElaCalendarPrivate::eventFilter(QObject* watched, QEvent* event)
{
    Q_Q(ElaCalendar);
    if (watched == _calendarView->viewport())
    {
        if (event->type() == QEvent::Wheel)
        {
            auto* wheelEvent = static_cast<QWheelEvent*>(event);
            if (wheelEvent->phase() == Qt::ScrollBegin)
            {
                _wheelRemainder = 0;
            }
            const int height = _calendarDelegate->getItemHeight();
            auto* scrollBar = static_cast<ElaScrollBar*>(_calendarView->verticalScrollBar());
            const bool isPageScroll = wheelEvent->modifiers().testFlag(Qt::ControlModifier) || wheelEvent->modifiers().testFlag(Qt::ShiftModifier);
            const qreal distance = !wheelEvent->pixelDelta().isNull() ? -wheelEvent->pixelDelta().y() : -wheelEvent->angleDelta().y() / 120.0 * (isPageScroll ? scrollBar->pageStep() : QApplication::wheelScrollLines() * height);
            _wheelRemainder += distance;
            const int rows = qRound(_wheelRemainder / height);
            _wheelRemainder -= rows * height;
            if (rows != 0)
            {
                _scrollAnimation->stop();
                _pageDate = QDate();
                int base = _wheelAnimation->state() == QAbstractAnimation::Running ? qRound(_wheelAnimation->getTargetPosition().x()) : scrollBar->value();
                if ((base - scrollBar->value()) * rows < 0 || qAbs(base - scrollBar->value()) > qAbs(rows * height) * scrollBar->getSpeedLimit())
                {
                    base = scrollBar->value();
                }
                const int target = qBound(scrollBar->minimum(), (qRound(base / static_cast<qreal>(height)) + rows) * height, scrollBar->maximum());
                if (ElaColorAnimation::getIsAnimationEnabled(q))
                {
                    scrollBar->scrollTo(target);
                }
                else
                {
                    scrollBar->stopSmoothScroll();
                    scrollBar->setValue(target);
                }
                if (target == scrollBar->minimum() || target == scrollBar->maximum())
                {
                    _wheelRemainder = 0;
                }
            }
            if (wheelEvent->phase() == Qt::ScrollEnd)
            {
                _wheelRemainder = 0;
            }
            event->accept();
            return true;
        }
        if (event->type() == QEvent::Hide)
        {
            _wheelRemainder = 0;
        }
    }
    return QObject::eventFilter(watched, event);
}

QDate ElaCalendarPrivate::_getPageStartDate(QDate date) const
{
    switch (_calendarModel->getDisplayMode())
    {
    case YearMode:
    {
        return QDate(qMax(1, date.year() - date.year() % 10 - 2), 1, 1);
    }
    case MonthMode:
    {
        return QDate(date.year(), 1, 1);
    }
    case DayMode:
    {
        const QDate first(date.year(), date.month(), 1);
        return first.addDays(-(first.dayOfWeek() % 7));
    }
    }
    return date;
}

void ElaCalendarPrivate::_scrollToDate(QDate date)
{
    const QScopedValueRollback<bool> updating(_isUpdatingDateWindow, true);
    date = qBound(_calendarModel->getMinimumDate(), date, _calendarModel->getMaximumDate());
    _scrollAnimation->stop();
    static_cast<ElaScrollBar*>(_calendarView->verticalScrollBar())->stopSmoothScroll();
    _wheelRemainder = 0;
    _pageDate = QDate();
    _calendarModel->ensureDateVisible(date, true);
    _calendarView->doItemsLayout();
    const int row = qMax(0, _calendarModel->getIndexFromDate(_getPageStartDate(date)).row());
    _calendarView->scrollTo(_calendarModel->index(row), QAbstractItemView::PositionAtTop);
    _restoreSelection();
    _displayDate = date;
    _updateSwitchButtonText();
}

void ElaCalendarPrivate::_scrollPage(int direction)
{
    Q_Q(ElaCalendar);
    if (!_isSwitchAnimationFinished)
    {
        return;
    }
    const auto mode = _calendarModel->getDisplayMode();
    const QDate anchor = _scrollAnimation->state() == QAbstractAnimation::Running && _pageDate.isValid() ? _pageDate : _displayDate;
    const QDate target = qBound(_calendarModel->getMinimumDate(), mode == DayMode ? anchor.addMonths(direction) : anchor.addYears(direction * (mode == MonthMode ? 1 : 10)), _calendarModel->getMaximumDate());
    const auto firstIndex = _calendarView->indexAt(QPoint(5, 5));
    const QDate firstDate = _calendarModel->getDateFromIndex(firstIndex);
    const int offset = _calendarView->visualRect(firstIndex).top();
    const QScopedValueRollback<bool> updating(_isUpdatingDateWindow, true);
    _scrollAnimation->stop();
    static_cast<ElaScrollBar*>(_calendarView->verticalScrollBar())->stopSmoothScroll();
    _wheelRemainder = 0;
    _dateWindowTimer->stop();
    if (_calendarModel->ensureDateVisible(target, true))
    {
        _calendarView->doItemsLayout();
        _calendarView->scrollTo(_calendarModel->getIndexFromDate(firstDate), QAbstractItemView::PositionAtTop);
        _calendarView->verticalScrollBar()->setValue(_calendarView->verticalScrollBar()->value() - offset);
        _restoreSelection();
    }
    auto* scrollBar = _calendarView->verticalScrollBar();
    const int start = scrollBar->value();
    const int row = qMax(0, _calendarModel->getIndexFromDate(_getPageStartDate(target)).row());
    _calendarView->scrollTo(_calendarModel->index(row), QAbstractItemView::PositionAtTop);
    const int end = scrollBar->value();
    _pageDate = target;
    if (!ElaColorAnimation::getIsAnimationEnabled(q) || start == end)
    {
        _displayDate = target;
        _updateSwitchButtonText();
        return;
    }
    scrollBar->setValue(start);
    _scrollAnimation->setStartValue(start);
    _scrollAnimation->setEndValue(end);
    _scrollAnimation->start();
}

void ElaCalendarPrivate::_switchDisplayMode(ElaCalendarType mode, QDate date)
{
    Q_Q(ElaCalendar);
    if (!_isSwitchAnimationFinished)
    {
        return;
    }
    _dateWindowTimer->stop();
    _scrollAnimation->stop();
    _isZoomIn = mode > _calendarModel->getDisplayMode();
    const QRect contentRect(1, 44, q->width() - 2, q->height() - 45);
    _oldCalendarViewPix = _getContentPixmap(contentRect);
    _calendarModel->setDisplayMode(mode);
    _calendarTitleView->setVisible(mode == DayMode);
    q->layout()->activate();
    _scrollToDate(qBound(_calendarModel->getMinimumDate(), date, _calendarModel->getMaximumDate()));
    _newCalendarViewPix = _getContentPixmap(contentRect);
    if (!ElaColorAnimation::getIsAnimationEnabled(q))
    {
        _oldCalendarViewPix = QPixmap();
        _newCalendarViewPix = QPixmap();
        return;
    }
    auto titlePolicy = _calendarTitleView->sizePolicy();
    titlePolicy.setRetainSizeWhenHidden(mode == DayMode);
    _calendarTitleView->setSizePolicy(titlePolicy);
    _calendarView->hide();
    _calendarTitleView->hide();
    _isSwitchAnimationFinished = false;
    _isDrawNewPix = false;
    _zoomRatio = 1;
    _pixOpacity = 1;
    _switchAnimation->start();
}

QPixmap ElaCalendarPrivate::_getContentPixmap(const QRect& contentRect) const
{
    Q_Q(const ElaCalendar);
    const qreal dpr = qMax(1.0, q->devicePixelRatioF());
    QPixmap pixmap(qRound(contentRect.width() * dpr), qRound(contentRect.height() * dpr));
    pixmap.setDevicePixelRatio(dpr);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    // 仅渲染透明内容子视图 避免把日历外框与窗口默认底色烘入缩放纹理
    for (QWidget* view: {static_cast<QWidget*>(_calendarTitleView), static_cast<QWidget*>(_calendarView)})
    {
        if (view && view->isVisible())
        {
            view->render(&painter, view->pos() - contentRect.topLeft(), QRegion(), QWidget::DrawChildren);
        }
    }
    return pixmap;
}

void ElaCalendarPrivate::_restoreSelection()
{
    _calendarView->selectionModel()->setCurrentIndex(_calendarModel->getIndexFromDate(_pSelectedDate), QItemSelectionModel::ClearAndSelect);
}

void ElaCalendarPrivate::_updateNavigationIcons()
{
    Q_Q(ElaCalendar);
    for (auto* button: {_upButton, _downButton})
    {
        const QIcon source(button == _upButton ? ":/include/Image/Fluent/caret_up_filled.svg" : ":/include/Image/Fluent/caret_down_filled.svg");
        QIcon icon;
        for (auto mode: {QIcon::Normal, QIcon::Active, QIcon::Disabled})
        {
            const qreal ratio = q->devicePixelRatioF();
            QPixmap pixmap = source.pixmap(button->iconSize() * ratio);
            pixmap.setDevicePixelRatio(ratio);
            QPainter painter(&pixmap);
            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            painter.fillRect(pixmap.rect(), mode == QIcon::Disabled ? ElaThemeColor(_themeMode, BasicTextDisable) : mode == QIcon::Active ? ElaThemeColor(_themeMode, BasicText)
                                                                                                                                          : ElaThemeColor(_themeMode, BasicTextNoFocus));
            painter.end();
            icon.addPixmap(pixmap, mode);
        }
        button->setIcon(icon);
    }
}

void ElaCalendarPrivate::_updateSwitchButtonText()
{
    const QModelIndex index = _calendarView->indexAt(QPoint(5, _calendarView->viewport()->height() / 2));
    const QDate date = _calendarModel->getDateFromIndex(index);
    if (date.isValid() && !_isUpdatingDateWindow)
    {
        _displayDate = date;
    }
    const auto mode = _calendarModel->getDisplayMode();
    switch (mode)
    {
    case YearMode:
    {
        const int first = _displayDate.year() - _displayDate.year() % 10;
        _modeSwitchButton->setText(QString("%1 - %2").arg(first).arg(first + 9));
        break;
    }
    case MonthMode:
    {
        _modeSwitchButton->setText(QString("%1年").arg(_displayDate.year()));
        break;
    }
    case DayMode:
    {
        _modeSwitchButton->setText(QString("%1年%2月").arg(_displayDate.year()).arg(_displayDate.month()));
        break;
    }
    }
    _modeSwitchButton->setEnabled(mode != YearMode);
    const QDate first = _getPageStartDate(_displayDate);
    _upButton->setEnabled(first > _getPageStartDate(_calendarModel->getMinimumDate()));
    _downButton->setEnabled(first < _getPageStartDate(_calendarModel->getMaximumDate()));
    if (_calendarDelegate->getDisplayDate().month() != _displayDate.month() || _calendarDelegate->getDisplayDate().year() != _displayDate.year())
    {
        _calendarDelegate->setDisplayDate(_displayDate);
        _calendarView->viewport()->update();
    }
}
