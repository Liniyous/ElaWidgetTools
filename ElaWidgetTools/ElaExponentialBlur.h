#ifndef ELAEXPONENTIALBLUR_H
#define ELAEXPONENTIALBLUR_H

#include <QObject>

#include "ElaProperty.h"
#include "ElaSingleton.h"

class ElaExponentialBlurPrivate;
class ELA_EXPORT ElaExponentialBlur : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(ElaExponentialBlur)
    Q_Q_CREATE(ElaExponentialBlur)
private:
    explicit ElaExponentialBlur(QObject* parent = nullptr);
    ~ElaExponentialBlur();

public:
    static QPixmap doExponentialBlur(QImage img, const quint16& blurRadius);
};

#endif // ELAEXPONENTIALBLUR_H
