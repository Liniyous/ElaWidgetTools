#include "ElaCalendarPicker.h"

#include "ElaApplication.h"
#include "ElaCalendar.h"
#include "ElaCalendarPickerContainer.h"
#include "ElaCalendarPickerPrivate.h"
#include "ElaColorAnimation.h"
#include "ElaControlAnimation.h"
#include "ElaControlStyle.h"
#include "ElaTheme.h"

#include <QDate>
#include <QHBoxLayout>
#include <QPainter>
#include <QTimer>
#include <QVBoxLayout>
#include <QVariantAnimation>

ELA_PROPERTY_Q_CPP(ElaCalendarPicker, int, BorderRadius)
ElaCalendarPicker::ElaCalendarPicker(QWidget* parent)
    : QPushButton{parent}, d_ptr(new ElaCalendarPickerPrivate())
{
    Q_D(ElaCalendarPicker);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    setFixedSize(120, ElaWidgetToolsSize::DefaultControlHeight);
    setObjectName("ElaCalendarPicker");
    setMouseTracking(true);
    d->_calendarPickerContainer = new ElaCalendarPickerContainer(this);
    d->_calendar = new ElaCalendar(d->_calendarPickerContainer);
    QVBoxLayout* containerLayout = new QVBoxLayout(d->_calendarPickerContainer);
    const QMargins margins = eTheme->getShadowMargins();
    containerLayout->setContentsMargins(margins);
    containerLayout->addWidget(d->_calendar);
    d->_calendarPickerContainer->resize(d->_calendar->size() + QSize(margins.left() + margins.right(), margins.top() + margins.bottom()));
    d->_calendarPickerContainer->hide();
    d->_controlAnimation = new ElaControlAnimation(this);
    d->_popupDelayTimer = new QTimer(d);
    d->_popupDelayTimer->setSingleShot(true);
    d->_popupDelayTimer->setTimerType(Qt::PreciseTimer);
    d->_popupDelayTimer->setInterval(83);
    connect(d->_popupDelayTimer, &QTimer::timeout, d, &ElaCalendarPickerPrivate::onPopupDelayTimeout);
    d->_popupAnimation = new QVariantAnimation(d);
    d->_popupAnimation->setStartValue(0.0);
    d->_popupAnimation->setEndValue(1.0);
    QEasingCurve easing(QEasingCurve::BezierSpline);
    easing.addCubicBezierSegment(QPointF(0, 0), QPointF(0, 1), QPointF(1, 1));
    d->_popupAnimation->setEasingCurve(easing);
    connect(d->_popupAnimation, &QVariantAnimation::valueChanged, this, [d](const QVariant& value) {
        const qreal progress = value.toReal();
        if (d->_isClosing)
        {
            d->_calendarPickerContainer->setWindowOpacity(d->_popupStartOpacity * (1 - progress));
        }
        else
        {
            d->_calendarPickerContainer->move(d->_popupPosition + QPoint(0, qRound(d->_popupOffset * (1 - progress))));
            d->_calendarPickerContainer->setWindowOpacity(qMin(1.0, d->_popupAnimation->currentTime() / 83.0));
        }
    });
    connect(d->_popupAnimation, &QVariantAnimation::finished, this, [d]() {
        if (d->_isClosing)
        {
            d->_calendarPickerContainer->hide();
        }
    });
    installEventFilter(d);
    d->_calendarPickerContainer->installEventFilter(d);
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
    const bool isPressed = isDown() || d->_controlAnimation->getIsPressed();
    painter.setPen(QPen(ElaControlStyle::getControlBorder(baseRect, d->_themeMode, isEnabled() && !isPressed), 1));
    painter.setBrush(d->_controlAnimation->getBackgroundColor(!isEnabled() ? ElaThemeColor(d->_themeMode, BasicDisable) : isPressed ? ElaThemeColor(d->_themeMode, BasicPress)
                                                                  : underMouse()                                                    ? ElaThemeColor(d->_themeMode, BasicHover)
                                                                                                                                    : ElaThemeColor(d->_themeMode, BasicBase)));
    painter.drawRoundedRect(baseRect, d->_pBorderRadius, d->_pBorderRadius);

    // 日期绘制
    QDate selectedDate = getSelectedDate();
    QString date = QString("%1/%2/%3").arg(selectedDate.year()).arg(selectedDate.month()).arg(selectedDate.day());
    painter.setPen(!isEnabled() ? ElaThemeColor(d->_themeMode, BasicTextDisable) : isPressed ? ElaThemeColor(d->_themeMode, BasicTextNoFocus)
                                                                                             : ElaThemeColor(d->_themeMode, BasicText));
    QRect textRect = baseRect;
    textRect.adjust(10, 0, 0, 0);
    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, date);

    // 图标绘制
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(eApp->getFontPixelSize() + 4);
    painter.setFont(iconFont);
    painter.drawText(QRect(baseRect.right() - 25, 0, 15, height()), Qt::AlignVCenter | Qt::AlignRight, QChar(ElaIconType::CalendarRange));
    painter.restore();
}
