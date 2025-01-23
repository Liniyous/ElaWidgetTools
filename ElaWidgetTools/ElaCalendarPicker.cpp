#include "ElaCalendarPicker.h"

#include <QDate>
#include <QHBoxLayout>
#include <QPainter>
#include <QVBoxLayout>

#include "ElaCalendar.h"
#include "ElaCalendarPickerContainer.h"
#include "ElaCalendarPickerPrivate.h"
#include "ElaTheme.h"
Q_PROPERTY_CREATE_Q_CPP(ElaCalendarPicker, int, BorderRadius)
ElaCalendarPicker::ElaCalendarPicker(QWidget* parent)
    : QPushButton{parent}, d_ptr(new ElaCalendarPickerPrivate())
{
    Q_D(ElaCalendarPicker);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    setFixedSize(120, 30);
    setObjectName("ElaCalendarPicker");
    setMouseTracking(true);
    d->_calendarPickerContainer = new ElaCalendarPickerContainer(this);
    d->_calendarPickerContainer->resize(317, 352);
    d->_calendar = new ElaCalendar(d->_calendarPickerContainer);
    QVBoxLayout* containerLayout = new QVBoxLayout(d->_calendarPickerContainer);
    containerLayout->setContentsMargins(6, 6, 6, 6);
    containerLayout->addWidget(d->_calendar);
    d->_calendarPickerContainer->hide();
    connect(this, &QPushButton::clicked, d, &ElaCalendarPickerPrivate::onCalendarPickerClicked);

    connect(d->_calendar, &ElaCalendar::pSelectedDateChanged, d, &ElaCalendarPickerPrivate::onCalendarSelectedDateChanged);
    setSelectedDate(QDate::currentDate());

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaCalendarPicker::~ElaCalendarPicker()
{
}

void ElaCalendarPicker::setSelectedDate(QDate selectedDate)
{
    Q_D(ElaCalendarPicker);
    d->_calendar->setSelectedDate(selectedDate);
    Q_EMIT selectedDateChanged(selectedDate);
}

QDate ElaCalendarPicker::getSelectedDate() const
{
    Q_D(const ElaCalendarPicker);
    return d->_calendar->getSelectedDate();
}

void ElaCalendarPicker::paintEvent(QPaintEvent* event)
{
    Q_D(ElaCalendarPicker);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 边框和背景绘制
    QRect baseRect = rect();
    baseRect.adjust(1, 1, -1, -1);
    painter.setPen(ElaThemeColor(d->_themeMode, BasicBorder));
    painter.setBrush(underMouse() ? ElaThemeColor(d->_themeMode, BasicHover) : ElaThemeColor(d->_themeMode, BasicBase));
    painter.drawRoundedRect(baseRect, d->_pBorderRadius, d->_pBorderRadius);

    // 日期绘制
    QDate selectedDate = getSelectedDate();
    QString date = QString("%1/%2/%3").arg(selectedDate.year()).arg(selectedDate.month()).arg(selectedDate.day());
    painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
    QRect textRect = baseRect;
    textRect.adjust(10, 0, 0, 0);
    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, date);

    // 图标绘制
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(17);
    painter.setFont(iconFont);
    painter.drawText(QRect(baseRect.right() - 25, 0, 15, height()), Qt::AlignVCenter | Qt::AlignRight, QChar((unsigned short)ElaIconType::CalendarRange));
    painter.restore();
}
