#ifndef ELAACRYLICURLCARDPRIVATE_H
#define ELAACRYLICURLCARDPRIVATE_H

#include <QMap>
#include <QObject>
#include <QPixmap>

#include "Def.h"
#include "stdafx.h"
class ElaAcrylicUrlCard;
class ElaAcrylicUrlCardPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaAcrylicUrlCard)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(qreal, MainOpacity)
    Q_PROPERTY_CREATE_D(qreal, NoiseOpacity)
    Q_PROPERTY_CREATE_D(int, BrushAlpha)
    Q_PROPERTY_CREATE_D(QString, Title);
    Q_PROPERTY_CREATE_D(QString, SubTitle);
    Q_PROPERTY_CREATE_D(int, TitlePixelSize);
    Q_PROPERTY_CREATE_D(int, SubTitlePixelSize);
    Q_PROPERTY_CREATE_D(int, TitleSpacing);
    Q_PROPERTY_CREATE_D(QPixmap, CardPixmap);
    Q_PROPERTY_CREATE_D(QSize, CardPixmapSize);
    Q_PROPERTY_CREATE_D(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_D(ElaCardPixType::PixMode, CardPixMode);
    Q_PROPERTY_CREATE_D(QString, Url);

public:
    explicit ElaAcrylicUrlCardPrivate(QObject* parent = nullptr);
    ~ElaAcrylicUrlCardPrivate();

private:
    int _shadowBorderWidth{6};
    QPixmap _noisePix;
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAACRYLICURLCARDPRIVATE_H
