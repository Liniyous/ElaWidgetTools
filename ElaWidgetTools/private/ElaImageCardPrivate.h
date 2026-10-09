#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAIMAGECARDPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAIMAGECARDPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QImage>
#include <QObject>

#include "ElaWidgetToolsDef.h"

class ElaImageCard;
class ElaImageCardPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaImageCard)
    ELA_PROPERTY_D(QImage, CardImage);
    ELA_PROPERTY_D(int, BorderRadius)
    ELA_PROPERTY_D(bool, IsPreserveAspectCrop)
public:
    explicit ElaImageCardPrivate(QObject* parent = nullptr);
    ~ElaImageCardPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELAIMAGECARDPRIVATE_H_

