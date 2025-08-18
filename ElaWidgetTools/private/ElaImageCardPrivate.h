#ifndef ELAIMAGECARDPRIVATE_H
#define ELAIMAGECARDPRIVATE_H

#include <QImage>
#include <QObject>

#include "ElaDef.h"

class ElaImageCard;
class ElaImageCardPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaImageCard)
    Q_PROPERTY_CREATE_D(QImage, CardImage);
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(bool, IsPreserveAspectCrop)
    Q_PROPERTY_CREATE_D(qreal, MaximumAspectRatio)
public:
    explicit ElaImageCardPrivate(QObject* parent = nullptr);
    ~ElaImageCardPrivate();

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAIMAGECARDPRIVATE_H
