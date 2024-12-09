#include "ElaCentralStackedWidget.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
ElaCentralStackedWidget::ElaCentralStackedWidget(QWidget* parent)
    : QStackedWidget(parent)
{
    setObjectName("ElaCentralStackedWidget");
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, &ElaCentralStackedWidget::onThemeModeChanged);
}

ElaCentralStackedWidget::~ElaCentralStackedWidget()
{
}

void ElaCentralStackedWidget::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
    setStyleSheet(getStyleStr());
}

void ElaCentralStackedWidget::setIsTransparent(bool isTransparent)
{
    this->_isTransparent = isTransparent;
    setStyleSheet(getStyleStr());
    update();
}

bool ElaCentralStackedWidget::getIsTransparent() const
{
    return _isTransparent;
}

void ElaCentralStackedWidget::setIsHasRadius(bool isHasRadius)
{
    this->_isHasRadius = isHasRadius;
    setStyleSheet(getStyleStr());
    update();
}

QString ElaCentralStackedWidget::colorToRgbaString(const QColor &color)
{
    return QString("rgba(%1, %2, %3, %4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alphaF(), 0, 'f', 2);
}


QString ElaCentralStackedWidget::getStyleStr()
{
    if (_isTransparent)
        return "ElaCentralStackedWidget#ElaCentralStackedWidget{background-color: transparent;}";
    return QString("ElaCentralStackedWidget#ElaCentralStackedWidget{background-color: %1;border-top: 1px solid %2;"
                   "border-left: 1px solid %2;border-right: none;border-bottom: none;"
                   "border-top-left-radius: %3px;}")
        .arg(colorToRgbaString(ElaThemeColor(_themeMode, WindowCentralStackBase)))
        .arg(colorToRgbaString(ElaThemeColor(_themeMode, BasicPageLine)))
        .arg(_isHasRadius ? 10 : 0);
}
