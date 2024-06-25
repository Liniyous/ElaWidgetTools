#include "ElaLineEdit.h"

#include <QPainter>
#include <QPainterPath>
#include <QTimer>

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
    d->_pAwesome = ElaIconType::None;
    d->_pBorderRadius = 6;
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
    d->_pAwesome = awesome;
    d->_pBorderRadius = 6;
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
    Q_EMIT focusIn(this->text());
    QLineEdit::focusInEvent(event);
}
