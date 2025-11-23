#include "ElaRollerPrivate.h"
#include <QPropertyAnimation>

ElaRollerPrivate::ElaRollerPrivate(QObject* parent)
    : QObject(parent)
{
}

ElaRollerPrivate::~ElaRollerPrivate()
{
}

void ElaRollerPrivate::_scroll(int delta)
{
    int steps = delta / 120;
    _targetScrollOffset -= steps * _pItemHeight;
    _targetScrollOffset = qRound(_targetScrollOffset / _pItemHeight) * _pItemHeight;
    if (!_pIsEnableLoop)
    {
        if (_targetScrollOffset < 0)
        {
            _targetScrollOffset = 0;
        }
        if (_targetScrollOffset > (_pItemList.size() - 1) * _pItemHeight)
        {
            _targetScrollOffset = (_pItemList.size() - 1) * _pItemHeight;
        }
    }
    _scrollAnimation->stop();
    _scrollAnimation->setStartValue(_pScrollOffset);
    _scrollAnimation->setEndValue(_targetScrollOffset);
    _scrollAnimation->start();
}
