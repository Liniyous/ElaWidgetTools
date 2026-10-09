#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAINTERACTIVECARDPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAINTERACTIVECARDPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QSize>

#include "ElaWidgetToolsDef.h"
class ElaInteractiveCard;
class ElaInteractiveCardPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaInteractiveCard)
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY_D(QString, Title);
    ELA_PROPERTY_D(QString, SubTitle);
    ELA_PROPERTY_D(int, TitlePixelSize);
    ELA_PROPERTY_D(int, SubTitlePixelSize);
    ELA_PROPERTY_D(int, TitleSpacing);
    ELA_PROPERTY_D(QPixmap, CardPixmap);
    ELA_PROPERTY_D(QSize, CardPixmapSize);
    ELA_PROPERTY_D(int, CardPixmapBorderRadius)
    ELA_PROPERTY_D(ElaCardPixType::PixMode, CardPixMode);

public:
    explicit ElaInteractiveCardPrivate(QObject* parent = nullptr);
    ~ElaInteractiveCardPrivate();

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAINTERACTIVECARDPRIVATE_H_

