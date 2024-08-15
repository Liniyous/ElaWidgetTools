#include "ElaExponentialBlur.h"

#include <QPainter>
#include <QPixmap>

#include "ElaExponentialBlurPrivate.h"
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <cmath>
#endif
Q_SINGLETON_CREATE_CPP(ElaExponentialBlur)
ElaExponentialBlur::ElaExponentialBlur(QObject* parent)
    : QObject{parent}, d_ptr(new ElaExponentialBlurPrivate())
{
    Q_D(ElaExponentialBlur);
    d->q_ptr = this;
}

ElaExponentialBlur::~ElaExponentialBlur()
{
}

QPixmap ElaExponentialBlur::doExponentialBlur(QImage img, const quint16& blurRadius)
{
    QImage shadowImage = img.convertToFormat(QImage::Format_ARGB32);
    ElaExponentialBlur::getInstance()->d_ptr->_drawExponentialBlur(shadowImage, blurRadius);
    return QPixmap::fromImage(shadowImage);
}
