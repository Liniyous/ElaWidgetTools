#ifndef ELATHEME_H
#define ELATHEME_H

#include <QObject>

#include "ElaDef.h"
#include "ElaProperty.h"
#include "ElaSingleton.h"

#define eTheme ElaTheme::getInstance()
#define ElaThemeColor(themeMode, themeColor) eTheme->getThemeColor(themeMode, ElaThemeType::themeColor)
class QPainter;
class ElaThemePrivate;
class ELA_EXPORT ElaTheme : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaTheme)
    Q_SINGLETON_CREATE_H(ElaTheme)
private:
    explicit ElaTheme(QObject* parent = nullptr);
    ~ElaTheme();

public:
    void setThemeMode(ElaThemeType::ThemeMode themeMode);
    ElaThemeType::ThemeMode getThemeMode() const;

    void drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius);

    void setThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor, QColor newColor);
    const QColor& getThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor);
Q_SIGNALS:
    Q_SIGNAL void themeModeChanged(ElaThemeType::ThemeMode themeMode);
};

#endif // ELATHEME_H
