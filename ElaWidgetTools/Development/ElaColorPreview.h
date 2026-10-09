#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOLORPREVIEW_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOLORPREVIEW_H_

#include "ElaPropertyMacro.h"
#include <QWidget>

#include "ElaWidgetToolsDef.h"
class ElaColorPreview : public QWidget
{
    Q_OBJECT
    ELA_VARIABLE(QColor, BaseColor)
    ELA_VARIABLE(ElaThemeType::ThemeMode, ThemeMode)
public:
    explicit ElaColorPreview(QWidget* parent = nullptr);
    ~ElaColorPreview();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACOLORPREVIEW_H_

