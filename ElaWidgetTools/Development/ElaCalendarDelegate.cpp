#include "ElaCalendarDelegate.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaTheme.h"

#include <QPainter>
#include <QStyleOption>

ElaCalendarDelegate::ElaCalendarDelegate(ElaCalendarModel* calendarModel, QObject* parent)
    : QStyledItemDelegate{parent}
{
    _calendarModel = calendarModel;
    _pItemWidth = 42;
    _pItemHeight = 42;
    _nowDate = QDate::currentDate();
    _pDisplayDate = _nowDate;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
    connect(_calendarModel, &ElaCalendarModel::displayModeChanged, this, &ElaCalendarDelegate::onCalendarModelDisplayModeChanged);
}

ElaCalendarDelegate::~ElaCalendarDelegate()
{
}

void ElaCalendarDelegate::onCalendarModelDisplayModeChanged()
{
    const bool isDay = _calendarModel->getDisplayMode() == DayMode;
    _pItemWidth = isDay ? 42 : 74;
    _pItemHeight = isDay ? 42 : 73;
}

void ElaCalendarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const QVariant value = index.data(Qt::UserRole);
    if (!value.isValid())
    {
        return;
    }
    const auto data = value.value<ElaCalendarData>();
    const auto mode = _calendarModel->getDisplayMode();
    const bool isDay = mode == DayMode;
    const bool isEnabled = option.state.testFlag(QStyle::State_Enabled);
    const bool isHovered = isEnabled && option.state.testFlag(QStyle::State_MouseOver);
    const bool isPressed = isEnabled && index == _pPressIndex;
    const bool isSelected = isEnabled && option.state.testFlag(QStyle::State_Selected);
    const bool isToday = data.year == _nowDate.year() && (mode == YearMode || (data.month == _nowDate.month() && (mode == MonthMode || data.day == _nowDate.day())));
    const bool isOutOfScope = isDay ? data.year != _pDisplayDate.year() || data.month != _pDisplayDate.month() : mode == MonthMode ? data.year != _pDisplayDate.year()
                                                                                                                                   : data.year / 10 != _pDisplayDate.year() / 10;
    const qreal diameter = qMin(option.rect.width(), option.rect.height()) - (isDay ? 6 : 12);
    const qreal radius = diameter / 2.0;
    // 使用浮点矩形几何中心定位圆圈与文字 避免整数 QRect::center 向左上截断导致视觉偏心
    const QPointF center = QRectF(option.rect).center();
    const QRectF cell(center.x() - radius, center.y() - radius, diameter, diameter);
    const QColor accent = ElaThemeColor(_themeMode, PrimaryNormal);
    QColor background = isPressed ? ElaThemeColor(_themeMode, BasicPressAlpha) : isHovered ? ElaThemeColor(_themeMode, BasicHoverAlpha)
                                                                                           : QColor(Qt::transparent);
    if (isToday)
    {
        background = !isEnabled ? ElaThemeColor(_themeMode, PrimaryDisabled) : isPressed ? ElaThemeColor(_themeMode, PrimaryPress)
            : isHovered                                                                  ? ElaThemeColor(_themeMode, PrimaryHover)
                                                                                         : accent;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(isSelected ? QPen(accent, 1.5) : QPen(Qt::NoPen));
    painter->setBrush(ElaColorAnimation::getColor(option.widget, background, "CalendarBackground", index));
    painter->drawEllipse(cell);
    if (isToday && isSelected)
    {
        painter->setPen(QPen(ElaThemeColor(_themeMode, BasicTextInvert), 1.5));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(cell.adjusted(3, 3, -3, -3));
    }
    painter->setPen(!isEnabled ? ElaThemeColor(_themeMode, BasicTextDisable) : isToday ? ElaThemeColor(_themeMode, BasicTextInvert)
                        : isSelected                                                   ? accent
                        : isOutOfScope || isPressed                                    ? ElaThemeColor(_themeMode, BasicTextNoFocus)
                                                                                       : ElaThemeColor(_themeMode, BasicText));
    const QString text = mode == YearMode ? QString::number(data.year) : mode == MonthMode ? QString("%1月").arg(data.month)
                                                                                           : QString::number(data.day);
    painter->drawText(cell, Qt::AlignCenter, text);
    if (!data.desText.isEmpty())
    {
        QFont labelFont = painter->font();
        labelFont.setPixelSize(qMax(8, eApp->getFontPixelSize() - 5));
        painter->setFont(labelFont);
        painter->drawText(QRectF(cell.x(), cell.y() + 1, cell.width(), 12), Qt::AlignTop | Qt::AlignHCenter, data.desText);
    }
    painter->restore();
}

QSize ElaCalendarDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(_pItemWidth, _pItemHeight);
}
