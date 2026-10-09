#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLPAGEAREAPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLPAGEAREAPRIVATE_H_

#include "ElaPropertyMacro.h"
#include <QObject>

#include "ElaWidgetToolsDef.h"
class ElaScrollPageArea;
class ElaScrollPageAreaPrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaScrollPageArea)
    ELA_PROPERTY_D(int, BorderRadius)
public:
    explicit ElaScrollPageAreaPrivate(QObject* parent = nullptr);
    ~ElaScrollPageAreaPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELASCROLLPAGEAREAPRIVATE_H_

