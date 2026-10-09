#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAEXPONENTIALBLURPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAEXPONENTIALBLURPRIVATE_H_

#include <QObject>

#include "ElaWidgetToolsExport.h"
#include "ElaPropertyMacro.h"

class ElaExponentialBlur;
class ElaExponentialBlurPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaExponentialBlur)
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

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAEXPONENTIALBLURPRIVATE_H_

