#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATHEMEPRIVATE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATHEMEPRIVATE_H_

#include "ElaPropertyMacro.h"
#include "ElaWidgetToolsDef.h"

#include <QCache>
#include <QColor>
#include <QObject>
#include <QPixmap>

class ElaTheme;
class QPainterPath;
class ElaThemePrivate : public QObject
{
    Q_OBJECT
    ELA_PIMPL_D(ElaTheme)
public:
    explicit ElaThemePrivate(QObject* parent = nullptr);
    ~ElaThemePrivate();

private:
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
    QColor _lightThemeColorList[ElaThemeType::ThemeColorCount];
    QColor _darkThemeColorList[ElaThemeType::ThemeColorCount];
    QCache<QByteArray, QPixmap> _shadowCache{8192}; // 阴影缓存上限为 8 MiB 成本单位为 KiB

    QPixmap _getShadowPixmap(const QPainterPath& path, const QSize& size, qreal devicePixelRatio, int shadowBorderWidth, qreal alphaRatio, const QColor& shadowColor);
};

#endif // ELAWORKSPACE_ELAWIDGETTOOLS_PRIVATE_ELATHEMEPRIVATE_H_

