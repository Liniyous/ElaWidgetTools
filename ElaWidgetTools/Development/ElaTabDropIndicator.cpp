#include "ElaTabDropIndicator.h"

#include "ElaTheme.h"

#include <QPainter>
#include <QPropertyAnimation>

ElaTabDropIndicator::ElaTabDropIndicator(QWidget* parent)
    : QWidget(parent)
{
    setAccessibleName("标签拖放覆盖");
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    _geometryAnimation = new QPropertyAnimation(this, "geometry", this);
    _opacityAnimation = new QPropertyAnimation(this, "pOpacity", this);
    for (auto* animation: {_geometryAnimation, _opacityAnimation})
    {
        animation->setDuration(120);
        animation->setEasingCurve(QEasingCurve::OutCubic);
    }
    connect(_opacityAnimation, &QPropertyAnimation::finished, this, [this]() {
        if (_opacity == 0)
        {
            hide();
        }
    });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this]() {
        update();
    });
    hide();
}

void ElaTabDropIndicator::showTarget(const QRect& target)
{
    if (!isVisible())
    {
        _geometryAnimation->stop();
        setGeometry(target);
        setOpacity(0);
        show();
    }
    else if (geometry() != target && (_geometryAnimation->state() != QAbstractAnimation::Running || _geometryAnimation->endValue().toRect() != target))
    {
        _geometryAnimation->stop();
        _geometryAnimation->setStartValue(geometry());
        _geometryAnimation->setEndValue(target);
        _geometryAnimation->start();
    }
    raise();
    _animateOpacity(1);
}

void ElaTabDropIndicator::hideTarget(bool animated)
{
    _geometryAnimation->stop();
    if (animated && isVisible() && _opacity > 0)
    {
        _animateOpacity(0);
    }
    else
    {
        _opacityAnimation->stop();
        setOpacity(0);
        hide();
    }
}

qreal ElaTabDropIndicator::getOpacity() const
{
    return _opacity;
}

void ElaTabDropIndicator::setOpacity(qreal opacity)
{
    _opacity = opacity;
    update();
}

void ElaTabDropIndicator::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setOpacity(_opacity);
    painter.fillRect(rect(), ElaThemeColor(eTheme->getThemeMode(), BasicHoverAlpha));
}

void ElaTabDropIndicator::_animateOpacity(qreal opacity)
{
    if (_opacity == opacity)
    {
        _opacityAnimation->stop();
        return;
    }
    if (_opacityAnimation->state() == QAbstractAnimation::Running && _opacityAnimation->endValue().toReal() == opacity)
    {
        return;
    }
    _opacityAnimation->stop();
    _opacityAnimation->setStartValue(_opacity);
    _opacityAnimation->setEndValue(opacity);
    _opacityAnimation->start();
}
