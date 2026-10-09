#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAREMINDERCARDPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAREMINDERCARDPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QSize>

#include "ElaWidgetToolsDef.h"
class ElaReminderCard;
class ElaReminderCardPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaReminderCard)
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
    explicit ElaReminderCardPrivate(QObject* parent = nullptr);
    ~ElaReminderCardPrivate();

private:
    int _shadowBorderWidth{6};
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAREMINDERCARDPRIVATE_H_

