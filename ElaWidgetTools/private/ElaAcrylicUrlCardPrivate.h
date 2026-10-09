#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAACRYLICURLCARDPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAACRYLICURLCARDPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QMap>
#include <QObject>
#include <QPixmap>

#include "ElaWidgetToolsDef.h"
class ElaAcrylicUrlCard;
class ElaAcrylicUrlCardPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaAcrylicUrlCard)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY_D(qreal, MainOpacity)
    ELA_PROPERTY_D(qreal, NoiseOpacity)
    ELA_PROPERTY_D(int, BrushAlpha)
    ELA_PROPERTY_D(QString, Title);
    ELA_PROPERTY_D(QString, SubTitle);
    ELA_PROPERTY_D(int, TitlePixelSize);
    ELA_PROPERTY_D(int, SubTitlePixelSize);
    ELA_PROPERTY_D(int, TitleSpacing);
    ELA_PROPERTY_D(int, SubTitleSpacing);
    ELA_PROPERTY_D(QPixmap, CardPixmap);
    ELA_PROPERTY_D(QSize, CardPixmapSize);
    ELA_PROPERTY_D(int, CardPixmapBorderRadius)
    ELA_PROPERTY_D(ElaCardPixType::PixMode, CardPixMode);
    ELA_PROPERTY_D(QString, Url);

public:
    explicit ElaAcrylicUrlCardPrivate(QObject* parent = nullptr);
    ~ElaAcrylicUrlCardPrivate();

private:
    QPixmap _noisePix;
    ElaThemeType::ThemeMode _themeMode;
    qreal _borderWidth{1.5};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAACRYLICURLCARDPRIVATE_H_

