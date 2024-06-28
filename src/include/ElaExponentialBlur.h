#ifndef ELAEXPONENTIALBLUR_H
#define ELAEXPONENTIALBLUR_H

#include <QObject>

#include "singleton.h"
#include "stdafx.h"
class ELA_EXPORT ElaExponentialBlur : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(ElaExponentialBlur)
private:
    explicit ElaExponentialBlur(QObject* parent = nullptr);
    ~ElaExponentialBlur();

public:
    static QPixmap doExponentialBlur(QImage img, const quint16& blurRadius);

private:
    static int _aprec;
    static int _zprec;
    static void _drawExponentialBlur(QImage& image, const quint16& qRadius);
    static void _drawRowBlur(QImage& image, const int& row, const int& alpha);
    static void _drawColumnBlur(QImage& image, const int& column, const int& alpha);
    static void _drawInnerBlur(unsigned char* bptr, int& zR, int& zG, int& zB, int& zA, int alpha);
};

#endif // ELAEXPONENTIALBLUR_H
