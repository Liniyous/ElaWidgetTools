#include "ElaRollerPickerPrivate.h"

#include "ElaRollerPicker.h"
#include <QPropertyAnimation>
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
    QPoint targetPos(q->mapToGlobal(QPoint(-6, (q->height() - _rollerPickerContainer->height() + _rollerPickerContainer->getButtonAreaHeight()) / 2)));
    _rollerPickerContainer->show();
    _rollerPickerContainer->setGeometry(QRect(targetPos, QSize(_getRollerTotalWidth() + 16, _pickerRollerHeight + _rollerPickerContainer->getButtonAreaHeight())));
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
