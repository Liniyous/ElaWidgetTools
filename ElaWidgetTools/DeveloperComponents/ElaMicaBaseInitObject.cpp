#include "ElaMicaBaseInitObject.h"

#include <QImage>

#include "ElaApplicationPrivate.h"
#include "ElaExponentialBlur.h"
ElaMicaBaseInitObject::ElaMicaBaseInitObject(ElaApplicationPrivate* appPrivate, QObject* parent)
    : QObject{parent}
{
    _appPrivate = appPrivate;
}

ElaMicaBaseInitObject::~ElaMicaBaseInitObject()
{
}

void ElaMicaBaseInitObject::onInitMicaBase(QImage img)
{
    // QColorDialog
    // 统一处理为1920*1080以节省空间
    img = img.scaled(QSize(1920, 1080), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QImage blurImage = ElaExponentialBlur::doExponentialBlur(img, 500).toImage();
    QImage lightImage = blurImage;
    QImage darkImage = blurImage;
    QColor lightMixColor = QColor(0xF3, 0xF3, 0xF3);
    lightMixColor = lightMixColor.toHsv();
    QColor darkMixColor = QColor(0x20, 0x20, 0x20);
    darkMixColor = darkMixColor.toHsv();
    QColor originColor;
    QColor lightColor;
    QColor darkColor;
    int h, s, v;
    for (int y = 0; y < blurImage.height(); y++)
    {
        QRgb* line = (QRgb*)blurImage.scanLine(y);
        for (int x = 0; x < blurImage.width(); x++)
        {
            originColor = QColor(line[x]);
            originColor = originColor.toHsv();
            h = originColor.hsvHue();
            s = originColor.hsvSaturation();
            v = originColor.value();
            if (s / 20 > 11)
            {
                lightColor.setHsv(h, (s / 20 + 11) / 2, 250);
            }
            else
            {
                lightColor.setHsv(h, 11, 250);
            }
            lightColor = lightColor.toRgb();
            if (v / 1.1 > 40)
            {
                darkColor.setHsv(h, s / 2, (v / 1.1 + 40) / 2);
            }
            else
            {
                darkColor.setHsv(h, s / 2, 40);
            }
            darkColor = darkColor.toRgb();
            lightImage.setPixel(x, y, qRgb(lightColor.red(), lightColor.green(), lightColor.blue()));
            darkImage.setPixel(x, y, qRgb(darkColor.red(), darkColor.green(), darkColor.blue()));
        }
    }
    _appPrivate->_lightBaseImage = lightImage.copy();
    _appPrivate->_darkBaseImage = darkImage.copy();
    // _appPrivate->_lightBaseImage.save("light.png", "PNG");
    // _appPrivate->_darkBaseImage.save("dark.png", "PNG");
    Q_EMIT initFinished();
}
