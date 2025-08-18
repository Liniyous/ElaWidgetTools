#ifndef ELAREMINDERCARDPRIVATE_H
#define ELAREMINDERCARDPRIVATE_H

#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QSize>

#include "ElaDef.h"
class ElaReminderCard;
class ElaReminderCardPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaReminderCard)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Title);
    Q_PROPERTY_CREATE_D(QString, SubTitle);
    Q_PROPERTY_CREATE_D(int, TitlePixelSize);
    Q_PROPERTY_CREATE_D(int, SubTitlePixelSize);
    Q_PROPERTY_CREATE_D(int, TitleSpacing);
    Q_PROPERTY_CREATE_D(QPixmap, CardPixmap);
    Q_PROPERTY_CREATE_D(QSize, CardPixmapSize);
    Q_PROPERTY_CREATE_D(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_D(ElaCardPixType::PixMode, CardPixMode);

public:
    explicit ElaReminderCardPrivate(QObject* parent = nullptr);
    ~ElaReminderCardPrivate();

private:
    int _shadowBorderWidth{6};
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAREMINDERCARDPRIVATE_H
