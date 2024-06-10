#include "ElaPushButton.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaApplication.h"
ElaPushButton::ElaPushButton(QWidget* parent)
    : QPushButton(parent)
{
    _pBorderRadius = 3;
    _themeMode = ElaApplication::getInstance()->getThemeMode();

    _pLightDefaultColor = QColor(0xFE, 0xFE, 0xFE);
    _pDarkDefaultColor = QColor(0x3E, 0x3E, 0x3E);
    _pLightHoverColor = QColor(0xF6, 0xF6, 0xF6);
    _pDarkHoverColor = QColor(0x4F, 0x4F, 0x4F);
    _pLightPressColor = QColor(0xF2, 0xF2, 0xF2);
    _pDarkPressColor = QColor(0x1C, 0x1C, 0x1C);
    _lightTextColor = Qt::black;
    _darkTextColor = Qt::white;
    setMouseTracking(true);
    setFixedSize(90, 38);
    setText("PushButton");
    QFont font = this->font();
    font.setPointSize(11);
    setFont(font);
    setObjectName("ElaPushButton");
    setStyleSheet("#ElaPushButton{background-color:transparent;}");
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, &ElaPushButton::onThemeChanged);
}

ElaPushButton::ElaPushButton(QString text, QWidget* parent)
    : QPushButton(text, parent)
{
    _pBorderRadius = 3;
    _themeMode = ElaApplication::getInstance()->getThemeMode();
    onThemeChanged(_themeMode);
    _pLightDefaultColor = QColor(0xFE, 0xFE, 0xFE);
    _pDarkDefaultColor = QColor(0x3E, 0x3E, 0x3E);
    _pLightHoverColor = QColor(0xF6, 0xF6, 0xF6);
    _pDarkHoverColor = QColor(0x4F, 0x4F, 0x4F);
    _pLightPressColor = QColor(0xF2, 0xF2, 0xF2);
    _pDarkPressColor = QColor(0x1C, 0x1C, 0x1C);
    _lightTextColor = Qt::black;
    _darkTextColor = Qt::white;
    if (_themeMode == ElaApplicationType::Dark)
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::ButtonText, _darkTextColor);
        setPalette(palette);
    }
    setMouseTracking(true);
    setFixedSize(90, 38);
    QFont font = this->font();
    font.setPointSize(11);
    setFont(font);
    setObjectName("ElaPushButton");
    setStyleSheet("#ElaPushButton{background-color:transparent;}");
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, &ElaPushButton::onThemeChanged);
}

ElaPushButton::~ElaPushButton()
{
}

void ElaPushButton::setLightTextColor(QColor color)
{
    _lightTextColor = color;
    if (_themeMode == ElaApplicationType::Light)
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::ButtonText, color);
        setPalette(palette);
    }
}

QColor ElaPushButton::getLightTextColor() const
{
    return _lightTextColor;
}

void ElaPushButton::setDarkTextColor(QColor color)
{
    _darkTextColor = color;
    if (_themeMode == ElaApplicationType::Dark)
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::ButtonText, color);
        setPalette(palette);
    }
}

QColor ElaPushButton::getDarkTextColor() const
{
    return _darkTextColor;
}

void ElaPushButton::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    _themeMode = themeMode;
    if (_themeMode == ElaApplicationType::Light)
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::ButtonText, _lightTextColor);
        setPalette(palette);
    }
    else
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::ButtonText, _darkTextColor);
        setPalette(palette);
    }
}

void ElaPushButton::mousePressEvent(QMouseEvent* event)
{
    _isPressed = true;
    QPushButton::mouseReleaseEvent(event);
}

void ElaPushButton::mouseReleaseEvent(QMouseEvent* event)
{
    _isPressed = false;
    Q_EMIT clicked();
    QPushButton::mouseReleaseEvent(event);
}

void ElaPushButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    if (_themeMode == ElaApplicationType::Light)
    {
        painter.save();
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        QColor color = ElaApplication::getInstance()->getShadowEffectColor();
        for (int i = 0; i < this->_shadowBorderWidth; i++)
        {
            QPainterPath path;
            path.setFillRule(Qt::WindingFill);
            path.addRoundedRect(this->_shadowBorderWidth - i, this->_shadowBorderWidth - i, this->width() - (this->_shadowBorderWidth - i) * 2, this->height() - (this->_shadowBorderWidth - i) * 2, this->_pBorderRadius + i, this->_pBorderRadius + i);
            int alpha = 6 * (_shadowBorderWidth - i + 1);
            color.setAlpha(alpha > 255 ? 255 : alpha);
            painter.setPen(color);
            painter.drawPath(path);
        }
        painter.restore();
    }
    // 背景绘制
    painter.save();
    QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth, width() - 2 * (_shadowBorderWidth), height() - 2 * _shadowBorderWidth);
    if (_themeMode == ElaApplicationType::Light)
    {
        painter.setPen(QPen(QColor(0xDF, 0xDF, 0xDF), 1));
        painter.setBrush(_isPressed ? _pLightPressColor : (underMouse() ? _pLightHoverColor : _pLightDefaultColor));
    }
    else
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(_isPressed ? _pDarkPressColor : (underMouse() ? _pDarkHoverColor : _pDarkDefaultColor));
    }
    painter.drawRoundedRect(foregroundRect, _pBorderRadius, _pBorderRadius);
    // 底边线绘制
    if (!_isPressed)
    {
        painter.setPen(QPen(QColor(0xBC, 0xBC, 0xBC), 1));
        painter.drawLine(foregroundRect.x() + _pBorderRadius, height() - _shadowBorderWidth, foregroundRect.width(), height() - _shadowBorderWidth);
    }
    painter.restore();
    QPushButton::paintEvent(event);
}
