#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELATHEME_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELATHEME_H_

#include "ElaPropertyMacro.h"
#include "ElaSingletonMacro.h"
#include "ElaWidgetToolsDef.h"
#include "ElaWidgetToolsExport.h"
#include <QColor>
#include <QMargins>
#include <QObject>

#define eTheme ElaTheme::getInstance()
#define ElaThemeColor(themeMode, themeColor) eTheme->getThemeColor(themeMode, ElaThemeType::themeColor)
class QPainter;
class QPainterPath;
class ElaThemePrivate;
class ELA_EXPORT ElaTheme : public QObject
{
    Q_OBJECT
    ELA_PIMPL_Q(ElaTheme)
    ELA_SINGLETON_H(ElaTheme)
private:
    explicit ElaTheme(QObject* parent = nullptr);
    ~ElaTheme();

public:
    void setThemeMode(ElaThemeType::ThemeMode themeMode);
    ElaThemeType::ThemeMode getThemeMode() const;

    QMargins getShadowMargins(int shadowBorderWidth = 32) const;
    void drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius, qreal alphaRatio = 1.0, const QColor& shadowColor = {});
    void drawEffectShadow(QPainter* painter, QRect widgetRect, const QMargins& shadowMargins, int borderRadius, qreal alphaRatio = 1.0, const QColor& shadowColor = {});
    // 按完整内容轮廓绘制阴影 shadowBorderWidth 为外侧最大留白 单位为逻辑像素
    void drawEffectShadow(QPainter* painter, const QPainterPath& contentPath, int shadowBorderWidth = 32, qreal alphaRatio = 1.0, const QColor& shadowColor = {});

    void setThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor, const QColor& newColor);
    const QColor& getThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor);
Q_SIGNALS:
    Q_SIGNAL void themeModeChanged(ElaThemeType::ThemeMode themeMode);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_ELATHEME_H_
