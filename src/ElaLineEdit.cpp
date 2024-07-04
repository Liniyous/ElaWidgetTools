#include "ElaLineEdit.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "ElaApplication.h"
#include "ElaEventBus.h"
#include "private/ElaLineEditPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaLineEdit, ElaIconType, Awesome)
Q_PROPERTY_CREATE_Q_CPP(ElaLineEdit, int, BorderRadius)

ElaLineEdit::ElaLineEdit(QWidget* parent)
    : QLineEdit(parent), d_ptr(new ElaLineEditPrivate())
{
    Q_D(ElaLineEdit);
    d->q_ptr = this;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    d->_pAwesome = ElaIconType::None;
    d->_pBorderRadius = 6;
    d->_pExpandMarkWidth = 0;
    // 事件总线
    d->_focusEvent = new ElaEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();
    d->_initStyle();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, d, &ElaLineEditPrivate::onThemeChanged);
}

ElaLineEdit::ElaLineEdit(ElaIconType awesome, QWidget* parent)
    : QLineEdit(parent), d_ptr(new ElaLineEditPrivate())
{
    Q_D(ElaLineEdit);
    d->q_ptr = this;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    d->_pAwesome = awesome;
    d->_pBorderRadius = 6;
    d->_pExpandMarkWidth = 0;
    // 事件总线
    d->_focusEvent = new ElaEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();
    d->_initStyle();
}

ElaLineEdit::~ElaLineEdit()
{
}

void ElaLineEdit::focusInEvent(QFocusEvent* event)
{
    Q_D(ElaLineEdit);
    Q_EMIT focusIn(this->text());
    QPropertyAnimation* markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
    connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    markAnimation->setDuration(300);
    markAnimation->setEasingCurve(QEasingCurve::InOutSine);
    markAnimation->setStartValue(d->_pExpandMarkWidth);
    markAnimation->setEndValue(width() / 2 - d->_pBorderRadius / 2);
    markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QLineEdit::focusInEvent(event);
}

void ElaLineEdit::focusOutEvent(QFocusEvent* event)
{
    Q_D(ElaLineEdit);
    Q_EMIT focusOut(this->text());
    QPropertyAnimation* markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
    connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    markAnimation->setDuration(300);
    markAnimation->setEasingCurve(QEasingCurve::InOutSine);
    markAnimation->setStartValue(d->_pExpandMarkWidth);
    markAnimation->setEndValue(0);
    markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QLineEdit::focusOutEvent(event);
}

void ElaLineEdit::paintEvent(QPaintEvent* event)
{
    Q_D(ElaLineEdit);
    QLineEdit::paintEvent(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(d->_themeMode == ElaApplicationType::Light ? QColor(0x0E, 0x6F, 0xC3) : QColor(0x4C, 0xA0, 0xE0));
    painter.drawRoundedRect(QRectF(width() / 2 - d->_pExpandMarkWidth, height() - 2.5, d->_pExpandMarkWidth * 2, 2.5), 2, 2);
    painter.restore();
}
