#include "ElaExponentialBlurPrivate.h"

#include <QPixmap>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <cmath>
#endif
int ElaExponentialBlurPrivate::_aprec = 12;
int ElaExponentialBlurPrivate::_zprec = 7;
ElaExponentialBlurPrivate::ElaExponentialBlurPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaExponentialBlurPrivate::~ElaExponentialBlurPrivate()
{
}

void ElaExponentialBlurPrivate::_drawExponentialBlur(QImage& image, const quint16& qRadius)
{
    if (qRadius < 1)
    {
        return;
    }
    image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    int alpha = (int)((1 << _aprec) * (1.0f - std::exp(-2.3f / (qRadius + 1.f))));
    int height = image.height();
    int width = image.width();
    for (int row = 0; row < height; row++)
    {
        _drawRowBlur(image, row, alpha);
    }

    for (int col = 0; col < width; col++)
    {
        _drawColumnBlur(image, col, alpha);
    }
}

void ElaExponentialBlurPrivate::_drawRowBlur(QImage& image, const int& row, const int& alpha)
{
    int zR, zG, zB, zA;

    QRgb* ptr = (QRgb*)image.scanLine(row);
    int width = image.width();

    zR = *((unsigned char*)ptr) << _zprec;
    zG = *((unsigned char*)ptr + 1) << _zprec;
    zB = *((unsigned char*)ptr + 2) << _zprec;
    zA = *((unsigned char*)ptr + 3) << _zprec;

    for (int index = 0; index < width; index++)
    {
        _drawInnerBlur((unsigned char*)&ptr[index], zR, zG, zB, zA, alpha);
    }
    for (int index = width - 2; index >= 0; index--)
    {
        _drawInnerBlur((unsigned char*)&ptr[index], zR, zG, zB, zA, alpha);
    }
}

void ElaExponentialBlurPrivate::_drawColumnBlur(QImage& image, const int& column, const int& alpha)
{
    int zR, zG, zB, zA;

    QRgb* ptr = (QRgb*)image.bits();
    ptr += column;
    int height = image.height();
    int width = image.width();

    zR = *((unsigned char*)ptr) << _zprec;
    zG = *((unsigned char*)ptr + 1) << _zprec;
    zB = *((unsigned char*)ptr + 2) << _zprec;
    zA = *((unsigned char*)ptr + 3) << _zprec;

    for (int index = width; index < (height - 1) * width; index += width)
    {
        _drawInnerBlur((unsigned char*)&ptr[index], zR, zG, zB, zA, alpha);
    }
    for (int index = (height - 2) * width; index >= 0; index -= width)
    {
        _drawInnerBlur((unsigned char*)&ptr[index], zR, zG, zB, zA, alpha);
    }
}

void ElaExponentialBlurPrivate::_drawInnerBlur(unsigned char* bptr, int& zR, int& zG, int& zB, int& zA, int alpha)
{
    int R, G, B, A;
    R = *bptr;
    G = *(bptr + 1);
    B = *(bptr + 2);
    A = *(bptr + 3);

    zR += (alpha * ((R << _zprec) - zR)) >> _aprec;
    zG += (alpha * ((G << _zprec) - zG)) >> _aprec;
    zB += (alpha * ((B << _zprec) - zB)) >> _aprec;
    zA += (alpha * ((A << _zprec) - zA)) >> _aprec;

    *bptr = zR >> _zprec;
    *(bptr + 1) = zG >> _zprec;
    *(bptr + 2) = zB >> _zprec;
    *(bptr + 3) = zA >> _zprec;
}
