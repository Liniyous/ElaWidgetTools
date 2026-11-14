#include "ElaCalendarModel.h"

ElaCalendarModel::ElaCalendarModel(QObject* parent)
    : QAbstractListModel{parent}
{
    _pMinimumDate.setDate(1924, 1, 1);
    _pMaximumDate.setDate(2124, 12, 31);
    _windowStartDate = _getWindowStartDate(QDate::currentDate());
    _updateRowCount();
}

ElaCalendarModel::~ElaCalendarModel()
{
}

void ElaCalendarModel::setMinimumDate(QDate minimumDate)
{
    if (!minimumDate.isValid() || minimumDate > _pMaximumDate || minimumDate == _pMinimumDate)
    {
        return;
    }
    beginResetModel();
    _pMinimumDate = minimumDate;
    _windowStartDate = _getWindowStartDate(_windowStartDate);
    _updateRowCount();
    endResetModel();
}

QDate ElaCalendarModel::getMinimumDate() const
{
    return _pMinimumDate;
}

void ElaCalendarModel::setMaximumDate(QDate maximumDate)
{
    if (!maximumDate.isValid() || maximumDate < _pMinimumDate || maximumDate == _pMaximumDate)
    {
        return;
    }
    beginResetModel();
    _pMaximumDate = maximumDate;
    _windowStartDate = _getWindowStartDate(_windowStartDate);
    _updateRowCount();
    endResetModel();
}

QDate ElaCalendarModel::getMaximumDate() const
{
    return _pMaximumDate;
}

void ElaCalendarModel::setDisplayMode(ElaCalendarType displayMode)
{
    if (_displayMode == displayMode)
    {
        return;
    }
    const QDate date = getDateFromIndex(index(_rowCount / 2));
    beginResetModel();
    _displayMode = displayMode;
    _windowStartDate = _getWindowStartDate(date);
    _updateRowCount();
    Q_EMIT displayModeChanged();
    endResetModel();
}

ElaCalendarType ElaCalendarModel::getDisplayMode() const
{
    return _displayMode;
}

QModelIndex ElaCalendarModel::getIndexFromDate(QDate date)
{
    if (!date.isValid())
    {
        return {};
    }
    int row = 0;
    switch (_displayMode)
    {
    case YearMode:
    {
        row = date.year() - _windowStartDate.year();
        break;
    }
    case MonthMode:
    {
        row = (date.year() - _windowStartDate.year()) * 12 + date.month() - _windowStartDate.month();
        break;
    }
    case DayMode:
    {
        row = static_cast<int>(_windowStartDate.daysTo(date));
        break;
    }
    }
    return row >= 0 && row < _rowCount ? index(row) : QModelIndex();
}

QDate ElaCalendarModel::getDateFromIndex(const QModelIndex& index) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _rowCount)
    {
        return {};
    }
    switch (_displayMode)
    {
    case YearMode:
    {
        return _windowStartDate.addYears(index.row());
    }
    case MonthMode:
    {
        return _windowStartDate.addMonths(index.row());
    }
    case DayMode:
    {
        const QDate date = _windowStartDate.addDays(index.row());
        return date >= _pMinimumDate && date <= _pMaximumDate ? date : QDate();
    }
    }
    return {};
}

bool ElaCalendarModel::ensureDateVisible(QDate date, bool isAlignToPage)
{
    if (!date.isValid())
    {
        return false;
    }
    const QModelIndex dateIndex = getIndexFromDate(date);
    // 日期窗口为六行视口预留三行提前加载的距离
    const int margin = _displayMode == DayMode ? 63 : 24;
    const int firstYear = qMax(1, date.year() - date.year() % 10 - 2);
    const bool isYearGridAligned = _displayMode != YearMode || !isAlignToPage || (firstYear - _windowStartDate.year()) % 4 == 0;
    if (dateIndex.isValid() && dateIndex.row() >= margin && dateIndex.row() < _rowCount - margin && isYearGridAligned)
    {
        return false;
    }
    QDate startDate = _getWindowStartDate(date);
    if (_displayMode == YearMode)
    {
        const int offset = (startDate.year() - (isAlignToPage ? firstYear : _windowStartDate.year())) % 4;
        startDate = QDate(qMax(1, startDate.year() - (offset + 4) % 4), 1, 1);
    }
    if (startDate == _windowStartDate)
    {
        return false;
    }
    beginResetModel();
    _windowStartDate = startDate;
    _updateRowCount();
    endResetModel();
    return true;
}

QVariant ElaCalendarModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::UserRole)
    {
        return {};
    }
    const QDate date = getDateFromIndex(index);
    if (!date.isValid())
    {
        return {};
    }
    QString description;
    if (_displayMode == DayMode && date.day() == 1)
    {
        description = QString("%1月").arg(date.month());
    }
    return QVariant::fromValue(ElaCalendarData(date.year(), date.month(), date.day(), description));
}

int ElaCalendarModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : _rowCount;
}

Qt::ItemFlags ElaCalendarModel::flags(const QModelIndex& index) const
{
    const QDate date = getDateFromIndex(index);
    const QDate last = _displayMode == YearMode ? date.addYears(1).addDays(-1) : _displayMode == MonthMode ? date.addMonths(1).addDays(-1)
                                                                                                           : date;
    return date.isValid() && last >= _pMinimumDate && date <= _pMaximumDate ? Qt::ItemIsEnabled | Qt::ItemIsSelectable : Qt::NoItemFlags;
}

QDate ElaCalendarModel::_getWindowStartDate(QDate date) const
{
    date = qBound(_pMinimumDate, date.isValid() ? date : QDate::currentDate(), _pMaximumDate);
    if (_displayMode == YearMode)
    {
        // 最后一窗预留最多三个前导年份 保证四列对齐后仍包含最大年份
        return QDate(qBound(_pMinimumDate.year(), date.year() - 72, qMax(_pMinimumDate.year(), _pMaximumDate.year() - 140)), 1, 1);
    }
    if (_displayMode == MonthMode)
    {
        return QDate(qBound(_pMinimumDate.year(), date.year() - 6, qMax(_pMinimumDate.year(), _pMaximumDate.year() - 11)), 1, 1);
    }
    // 仅承载附近 27 周 保持周日对齐 避免完整日期范围进入视图布局与系统查询
    const QDate firstMonth(_pMinimumDate.year(), _pMinimumDate.month(), 1);
    const QDate firstWeek = firstMonth.addDays(-(firstMonth.dayOfWeek() % 7));
    const QDate lastStart = _pMaximumDate.addDays(-(_pMaximumDate.dayOfWeek() % 7) - 26 * 7);
    return qBound(firstWeek, date.addDays(-(date.dayOfWeek() % 7) - 13 * 7), qMax(firstWeek, lastStart));
}

void ElaCalendarModel::_updateRowCount()
{
    switch (_displayMode)
    {
    case YearMode:
    {
        _rowCount = qMin(144, _pMaximumDate.year() - _windowStartDate.year() + 1);
        break;
    }
    case MonthMode:
    {
        _rowCount = qMin(144, (_pMaximumDate.year() - _windowStartDate.year() + 1) * 12);
        break;
    }
    case DayMode:
    {
        _rowCount = static_cast<int>(qMin<qint64>(189, _windowStartDate.daysTo(_pMaximumDate) + 1));
        break;
    }
    }
}
