#ifndef ELAWIDGETTOOLS_ELALCDNUMBERSTYLE_H
#define ELAWIDGETTOOLS_ELALCDNUMBERSTYLE_H
#include <QProxyStyle>

#include "ElaDef.h"
class ElaLCDNumberStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PRIVATE_CREATE(bool, IsTransparent)
public:
    explicit ElaLCDNumberStyle(QStyle* style = nullptr);
    ~ElaLCDNumberStyle();
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif //ELAWIDGETTOOLS_ELALCDNUMBERSTYLE_H
