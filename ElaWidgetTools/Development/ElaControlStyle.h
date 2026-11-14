#ifndef ELAWORKSPACE_ELAWIDGETTOOLS_ELACONTROLSTYLE_H_
#define ELAWORKSPACE_ELAWIDGETTOOLS_ELACONTROLSTYLE_H_

#include "ElaWidgetToolsDef.h"

#include <QBrush>
#include <QRectF>
#include <QStyle>

class QPainter;
class QStyleOptionTab;
class QWidget;

class ElaControlStyle
{
public:
    static QBrush getControlBorder(const QRectF& rect, ElaThemeType::ThemeMode themeMode, bool isElevated);
    static QColor getControlBackgroundColor(ElaThemeType::ThemeMode themeMode, QStyle::State state, bool isAccent = false);
    static QColor getIndicatorBackgroundColor(ElaThemeType::ThemeMode themeMode, QStyle::State state, bool isChecked);
    static QColor getItemBackgroundColor(ElaThemeType::ThemeMode themeMode, QStyle::State state, bool isPressed = false, bool isAlternate = false);
    static QColor getTabBackgroundColor(const QWidget* widget, const QStyleOptionTab* option, const QColor& target);
    static QColor getStatusColor(ElaThemeType::ThemeMode themeMode, ElaMessageBarType::MessageMode mode, bool isBackground = false);
    static void drawStatusIcon(QPainter* painter, const QRectF& rect, ElaThemeType::ThemeMode themeMode, ElaMessageBarType::MessageMode mode);
    static void drawInputFrame(QPainter* painter, const QRectF& rect, qreal radius, ElaThemeType::ThemeMode themeMode, QStyle::State state, const QWidget* widget = nullptr, qreal focusProgress = -1);
};

#endif //ELAWORKSPACE_ELAWIDGETTOOLS_ELACONTROLSTYLE_H_
