#include "ElaSlider.h"

#include "ElaSliderStyle.h"

#include <QEvent>

ElaSlider::ElaSlider(QWidget* parent)
    : QSlider(parent)
{
    setOrientation(Qt::Horizontal);
    auto* sliderStyle = new ElaSliderStyle(style());
    sliderStyle->setParent(this);
    setStyle(sliderStyle);
}

ElaSlider::ElaSlider(Qt::Orientation orientation, QWidget* parent)
    : ElaSlider(parent)
{
    setOrientation(orientation);
}

ElaSlider::~ElaSlider() = default;
