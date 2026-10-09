#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACHECKBOXSTYLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACHECKBOXSTYLE_H_

#include <QProxyStyle>

#include "ElaWidgetToolsDef.h"
class ElaCheckBoxStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaCheckBoxStyle(QStyle* style = nullptr);
    ~ElaCheckBoxStyle();
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;
    QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    static constexpr int _labelSpacing{10};
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_DEVELOPERCOMPONENTS_ELACHECKBOXSTYLE_H_
