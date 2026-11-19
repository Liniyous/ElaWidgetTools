#include "ElaRollerPickerPrivate.h"

#include "ElaRollerPicker.h"
#include "ElaTheme.h"

#include <QGuiApplication>
#include <QPropertyAnimation>
#include <QScreen>
ElaRollerPickerPrivate::ElaRollerPickerPrivate(QObject* parent)
    : QObject(parent)
{
}

ElaRollerPickerPrivate::~ElaRollerPickerPrivate()
{
}

void ElaRollerPickerPrivate::onRollerPickerClicked()
{
    Q_Q(ElaRollerPicker);
    const QMargins contentMargins = _rollerPickerContainer->contentsMargins();
    const QSize size(_getRollerTotalWidth() + contentMargins.left() + contentMargins.right(), _pickerRollerHeight + contentMargins.top() + contentMargins.bottom());
    QPoint targetPos = q->mapToGlobal(QPoint(-eTheme->getShadowMargins().left(), q->rect().center().y() - (_pickerRollerHeight - 1) / 2 - contentMargins.top()));
    QScreen* screen = QGuiApplication::screenAt(q->mapToGlobal(q->rect().center()));
    if (screen)
    {
        const QRect available = screen->availableGeometry().adjusted(4, 4, -4, -4);
        targetPos.setX(qBound(available.left(), targetPos.x(), qMax(available.left(), available.right() - size.width() + 1)));
        targetPos.setY(qBound(available.top(), targetPos.y(), qMax(available.top(), available.bottom() - size.height() + 1)));
    }
    _rollerPickerContainer->setGeometry(QRect(targetPos, size));
    _rollerPickerContainer->show();
    _rollerPickerContainer->doPickerAnimation();
}

void ElaRollerPickerPrivate::onOverButtonClicked()
{
    Q_Q(ElaRollerPicker);
    Q_EMIT q->currentDataChanged(q->getCurrentData());
}

void ElaRollerPickerPrivate::onCancelButtonClicked()
{
}

int ElaRollerPickerPrivate::_getRollerTotalWidth() const
{
    int totalWidth = 0;
    for (auto rollerItem: _rollerPickerContainer->_rollerList)
    {
        totalWidth += rollerItem->width();
    }
    return totalWidth;
}
