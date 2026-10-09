#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELALCDNUMBERSTYLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELALCDNUMBERSTYLE_H_
#include "ElaPropertyMacro.h"
#include <QProxyStyle>

#include "ElaWidgetToolsDef.h"
class ElaLCDNumberStyle : public QProxyStyle
{
    Q_OBJECT
    ELA_VARIABLE(bool, IsTransparent)
public:
    explicit ElaLCDNumberStyle(QStyle* style = nullptr);
    ~ElaLCDNumberStyle();
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELALCDNUMBERSTYLE_H_

