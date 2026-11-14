#include "ElaMicaBaseInitObject.h"

#include "ElaExponentialBlurPrivate.h"

#include <QColor>

ElaMicaBaseInitObject::ElaMicaBaseInitObject(QObject* parent)
    : QObject{parent}
{
}

ElaMicaBaseInitObject::~ElaMicaBaseInitObject()
{
}

void ElaMicaBaseInitObject::onInitMicaBase(QImage image, quint64 revision)
{
    // 后台只操作局部图像 通过队列信号交回主线程发布缓存
    image = image.scaled(QSize(1920, 1080), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ElaExponentialBlurPrivate::_drawExponentialBlur(image, 500);
    QImage lightImage(image.size(), QImage::Format_RGB32);
    QImage darkImage(image.size(), QImage::Format_RGB32);
    if (lightImage.isNull() || darkImage.isNull())
    {
        Q_EMIT initFinished(QImage(), QImage(), revision);
        return;
    }
    for (int y = 0; y < image.height(); ++y)
    {
        const auto* sourceLine = reinterpret_cast<const QRgb*>(image.constScanLine(y));
        auto* lightLine = reinterpret_cast<QRgb*>(lightImage.scanLine(y));
        auto* darkLine = reinterpret_cast<QRgb*>(darkImage.scanLine(y));
        for (int x = 0; x < image.width(); ++x)
        {
            const QColor color = QColor::fromRgb(sourceLine[x]).toHsv();
            const int hue = color.hsvHue();
            const int saturation = color.hsvSaturation();
            const int value = color.value();
            lightLine[x] = QColor::fromHsv(hue, saturation / 20 > 11 ? (saturation / 20 + 11) / 2 : 11, 250).rgb();
            darkLine[x] = QColor::fromHsv(hue, saturation / 2, value / 1.1 > 40 ? static_cast<int>((value / 1.1 + 40) / 2) : 40).rgb();
        }
    }
    Q_EMIT initFinished(lightImage, darkImage, revision);
}
