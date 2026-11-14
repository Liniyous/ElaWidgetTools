#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELALINEEDITSTYLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELALINEEDITSTYLE_H_

#include "ElaPropertyMacro.h"

#include <QProxyStyle>

#include "ElaWidgetToolsDef.h"
class ElaLineEditStyle : public QProxyStyle
{
    Q_OBJECT
    ELA_PROPERTY(qreal, ExpandMarkProgress)
public:
    explicit ElaLineEditStyle(QStyle* style = nullptr);
    ~ElaLineEditStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELALINEEDITSTYLE_H_

