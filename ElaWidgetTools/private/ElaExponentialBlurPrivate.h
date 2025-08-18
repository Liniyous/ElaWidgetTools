#ifndef ELAEXPONENTIALBLURPRIVATE_H
#define ELAEXPONENTIALBLURPRIVATE_H

#include <QObject>

#include "ElaProperty.h"

class ElaExponentialBlur;
class ElaExponentialBlurPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaExponentialBlur)
public:
    explicit ElaExponentialBlurPrivate(QObject* parent = nullptr);
    ~ElaExponentialBlurPrivate();

private:
    static int _aprec;
    static int _zprec;
    static void _drawExponentialBlur(QImage& image, const quint16& qRadius);
    static void _drawRowBlur(QImage& image, const int& row, const int& alpha);
    static void _drawColumnBlur(QImage& image, const int& column, const int& alpha);
    static void _drawInnerBlur(unsigned char* bptr, int& zR, int& zG, int& zB, int& zA, int alpha);
};

#endif // ELAEXPONENTIALBLURPRIVATE_H
