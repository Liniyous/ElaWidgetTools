#ifndef ELAPROGRESSBARSTYLE_H
#define ELAPROGRESSBARSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaProgressBarStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaProgressBarStyle(QStyle* style = nullptr);
    ~ElaProgressBarStyle();
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAPROGRESSBARSTYLE_H
