#include "ElaWidgetPrivate.h"
#include "ElaApplication.h"
#include "ElaWidget.h"
#include "ElaTheme.h"

ElaWidgetPrivate::ElaWidgetPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaWidgetPrivate::~ElaWidgetPrivate()
{
}

void ElaWidgetPrivate::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaWidget);
    _themeMode = themeMode;
    if (eApp->getIsEnableMica())
        return;
    QPalette palette = q->palette();
    QString mode = eApp->getWindowAttribute();
    if (mode == "acrylic-material" || mode == "mica" || mode == "mica-alt") {
        palette.setBrush(QPalette::Window, Qt::transparent);
        _windowAgent->setWindowAttribute("dark-mode", themeMode == ElaThemeType::Dark);
    } else if (mode == "dwm-blur") {
        palette.setBrush(QPalette::Window, themeMode == ElaThemeType::Light ? QColor(0xF0, 0xF4 , 0xF9, 100) : QColor(0x20, 0x20, 0x20, 100));
    } else {
        palette.setBrush(QPalette::Window, ElaThemeColor(_themeMode, WindowBase));
    }
    _doWindowAttributeModeChange(mode);
    q->setPalette(palette);
}

void ElaWidgetPrivate::_doWindowAttributeModeChange(QString mode)
{
    if (_oldWindowAttribute == "acrylic-material" || _oldWindowAttribute == "mica"
        || _oldWindowAttribute == "mica-alt" || _oldWindowAttribute == "dwm-blur") {
        _windowAgent->setWindowAttribute(_oldWindowAttribute, false);
    }

    if (mode == "acrylic-material" || mode == "mica" || mode == "mica-alt" || mode == "dwm-blur") {
        _windowAgent->setWindowAttribute(mode, true);
    }
    _oldWindowAttribute = mode;
}
