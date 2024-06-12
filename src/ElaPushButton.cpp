#include "ElaPushButton.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaApplication.h"
#include "private/ElaPushButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, LightDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, DarkDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, LightHoverColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, DarkHoverColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, LightPressColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, DarkPressColor)
ElaPushButton::ElaPushButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaPushButtonPrivate())
{
    Q_D(ElaPushButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    d->onThemeChanged(d->_themeMode);
    d->_pLightDefaultColor = QColor(0xFE, 0xFE, 0xFE);
    d->_pDarkDefaultColor = QColor(0x3E, 0x3E, 0x3E);
    d->_pLightHoverColor = QColor(0xF6, 0xF6, 0xF6);
    d->_pDarkHoverColor = QColor(0x4F, 0x4F, 0x4F);
    d->_pLightPressColor = QColor(0xF2, 0xF2, 0xF2);
    d->_pDarkPressColor = QColor(0x1C, 0x1C, 0x1C);
    d->_lightTextColor = Qt::black;
    d->_darkTextColor = Qt::white;
    setMouseTracking(true);
    setFixedSize(90, 38);
    setText("PushButton");
    QFont font = this->font();
    font.setPointSize(11);
    setFont(font);
    setObjectName("ElaPushButton");
    setStyleSheet("#ElaPushButton{background-color:transparent;}");
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, d, &ElaPushButtonPrivate::onThemeChanged);
}

ElaPushButton::ElaPushButton(QString text, QWidget* parent)
    : QPushButton(text, parent), d_ptr(new ElaPushButtonPrivate())
{
    Q_D(ElaPushButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    d->onThemeChanged(d->_themeMode);
    d->_pLightDefaultColor = QColor(0xFE, 0xFE, 0xFE);
    d->_pDarkDefaultColor = QColor(0x3E, 0x3E, 0x3E);
    d->_pLightHoverColor = QColor(0xF6, 0xF6, 0xF6);
    d->_pDarkHoverColor = QColor(0x4F, 0x4F, 0x4F);
    d->_pLightPressColor = QColor(0xF2, 0xF2, 0xF2);
    d->_pDarkPressColor = QColor(0x1C, 0x1C, 0x1C);
    d->_lightTextColor = Qt::black;
    d->_darkTextColor = Qt::white;
    if (d->_themeMode == ElaApplicationType::Dark)
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::ButtonText, d->_darkTextColor);
        setPalette(palette);
    }
    setMouseTracking(true);
    setFixedSize(90, 38);
    QFont font = this->font();
    font.setPointSize(11);
    setFont(font);
    setObjectName("ElaPushButton");
    setStyleSheet("#ElaPushButton{background-color:transparent;}");
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, d, &ElaPushButtonPrivate::onThemeChanged);
}

ElaPushButton::~ElaPushButton()
{
}

void ElaPushButton::setLightTextColor(QColor color)
{
    Q_D(ElaPushButton);
    d->_lightTextColor = color;
    if (d->_themeMode == ElaApplicationType::Light)
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::ButtonText, color);
        setPalette(palette);
    }
}

QColor ElaPushButton::getLightTextColor() const
{
    Q_D(const ElaPushButton);
    return d->_lightTextColor;
}

void ElaPushButton::setDarkTextColor(QColor color)
{
    Q_D(ElaPushButton);
    d->_darkTextColor = color;
    if (d->_themeMode == ElaApplicationType::Dark)
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::ButtonText, color);
        setPalette(palette);
    }
}

QColor ElaPushButton::getDarkTextColor() const
{
    Q_D(const ElaPushButton);
    return d->_darkTextColor;
}

void ElaPushButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaPushButton);
    d->_isPressed = true;
    QPushButton::mouseReleaseEvent(event);
}

void ElaPushButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaPushButton);
    d->_isPressed = false;
    Q_EMIT clicked();
    QPushButton::mouseReleaseEvent(event);
}

void ElaPushButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaPushButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影

    painter.save();
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QColor color = d->_themeMode == ElaApplicationType::Light ? ElaApplication::getInstance()->getLightShadowEffectColor() : ElaApplication::getInstance()->getDarkShadowEffectColor();
    for (int i = 0; i < d->_shadowBorderWidth; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(d->_shadowBorderWidth - i, d->_shadowBorderWidth - i, this->width() - (d->_shadowBorderWidth - i) * 2, this->height() - (d->_shadowBorderWidth - i) * 2, d->_pBorderRadius + i, d->_pBorderRadius + i);
        int alpha = 5 * (d->_shadowBorderWidth - i + 1);
        color.setAlpha(alpha > 255 ? 255 : alpha);
        painter.setPen(color);
        painter.drawPath(path);
    }
    painter.restore();

    // 背景绘制
    painter.save();
    QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * (d->_shadowBorderWidth), height() - 2 * d->_shadowBorderWidth);
    if (d->_themeMode == ElaApplicationType::Light)
    {
        painter.setPen(QPen(QColor(0xDF, 0xDF, 0xDF), 1));
        painter.setBrush(d->_isPressed ? d->_pLightPressColor : (underMouse() ? d->_pLightHoverColor : d->_pLightDefaultColor));
    }
    else
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(d->_isPressed ? d->_pDarkPressColor : (underMouse() ? d->_pDarkHoverColor : d->_pDarkDefaultColor));
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 底边线绘制
    if (!d->_isPressed)
    {
        painter.setPen(QPen(QColor(0xBC, 0xBC, 0xBC), 1));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - d->_shadowBorderWidth, foregroundRect.width(), height() - d->_shadowBorderWidth);
    }
    painter.restore();
    QPushButton::paintEvent(event);
}
