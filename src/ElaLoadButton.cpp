#include "ElaLoadButton.h"

#include <QPainter>
#include <QPainterPath>
#include <QHBoxLayout>

#include "ElaTheme.h"
#include "private/ElaLoadButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaLoadButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaLoadButton, QColor, LightDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(ElaLoadButton, QColor, DarkDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(ElaLoadButton, QColor, LightHoverColor)
Q_PROPERTY_CREATE_Q_CPP(ElaLoadButton, QColor, DarkHoverColor)
Q_PROPERTY_CREATE_Q_CPP(ElaLoadButton, QColor, LightPressColor)
Q_PROPERTY_CREATE_Q_CPP(ElaLoadButton, QColor, DarkPressColor)
ElaLoadButton::ElaLoadButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaLoadButtonPrivate())
{
    Q_D(ElaLoadButton);
    d->q_ptr = this;

    setFixedHeight(38);

    d->_pBorderRadius = 3;
    d->_themeMode = eTheme->getThemeMode();
    d->_pLightDefaultColor = ElaThemeColor(ElaThemeType::Light, BasicBase);
    d->_pDarkDefaultColor = ElaThemeColor(ElaThemeType::Dark, BasicBase);
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, BasicHover);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, BasicHover);
    d->_pLightPressColor = ElaThemeColor(ElaThemeType::Light, BasicPress);
    d->_pDarkPressColor = ElaThemeColor(ElaThemeType::Dark, BasicPress);
    d->_lightTextColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_darkTextColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    setMouseTracking(true);

    setObjectName("ElaLoadButton");
    setStyleSheet("#ElaLoadButton{background-color:transparent;}");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        QColor textColor = (isEnabled() ? d->_themeMode == ElaThemeType::Light ? d->_lightTextColor : d->_darkTextColor : ElaThemeColor(d->_themeMode, BasicTextDisable));
        d->_text->setStyleSheet(QString("QLabel{color:%1}").arg(textColor.name()));
    });


    d->_outTimer = new QTimer(this);
    d->_outTimer->setSingleShot(true);
    connect(d->_outTimer, &QTimer::timeout, this, [=](){
        setEnabled(true);
        d->_progressRing->stop();
        d->_progressRing->setCustomBackgroundColor(Qt::transparent, Qt::transparent);
        emit timeout();
    });
    d->_text = new QLabel("登录", this);
    QFont font = d->_text->font();
    font.setPixelSize(15);
    d->_text->setFont(font);
    d->_progressRing = new ElaProgressRing(this, false);
    d->_progressRing->setStrokeWidth(3);
    d->_progressRing->setFixedSize(16, 16);
    QHBoxLayout* lay = new QHBoxLayout(this);
    lay->setSpacing(4);
    lay->setContentsMargins(19, 6, 4, 6);
    lay->addStretch();
    lay->addWidget(d->_text);
    lay->addWidget(d->_progressRing);
    lay->addStretch();

    QColor textColor = (isEnabled() ? d->_themeMode == ElaThemeType::Light ? d->_lightTextColor : d->_darkTextColor : ElaThemeColor(d->_themeMode, BasicTextDisable));
    d->_text->setStyleSheet(QString("QLabel{color:%1}").arg(textColor.name()));
}

ElaLoadButton::ElaLoadButton(QString text, QWidget* parent)
    : ElaLoadButton(parent)
{
    Q_D(ElaLoadButton);
    d->_text->setText(text);
    setText(text);
}

ElaLoadButton::~ElaLoadButton()
{
}

void ElaLoadButton::setLightTextColor(QColor color)
{
    Q_D(ElaLoadButton);
    d->_lightTextColor = color;
}

QColor ElaLoadButton::getLightTextColor() const
{
    Q_D(const ElaLoadButton);
    return d->_lightTextColor;
}

void ElaLoadButton::setDarkTextColor(QColor color)
{
    Q_D(ElaLoadButton);
    d->_darkTextColor = color;
}

QColor ElaLoadButton::getDarkTextColor() const
{
    Q_D(const ElaLoadButton);
    return d->_darkTextColor;
}

void ElaLoadButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaLoadButton);
    d->_isPressed = true;
    QPushButton::mousePressEvent(event);
}

void ElaLoadButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaLoadButton);
    d->_isPressed = false;
    QPushButton::mouseReleaseEvent(event);
}

void ElaLoadButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaLoadButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    // eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    // 背景绘制
    painter.save();
    QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * (d->_shadowBorderWidth), height() - 2 * d->_shadowBorderWidth);
    if (d->_themeMode == ElaThemeType::Light)
    {
        painter.setPen(ElaThemeColor(ElaThemeType::Light, BasicBorder));
        painter.setBrush(isEnabled() ? d->_isPressed ? d->_pLightPressColor : (underMouse() ? d->_pLightHoverColor : d->_pLightDefaultColor) : ElaThemeColor(d->_themeMode, BasicDisable));
    }
    else
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(isEnabled() ? d->_isPressed ? d->_pDarkPressColor : (underMouse() ? d->_pDarkHoverColor : d->_pDarkDefaultColor) : ElaThemeColor(d->_themeMode, BasicDisable));
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 底边线绘制
    if (!d->_isPressed)
    {
        painter.setPen(ElaThemeColor(d->_themeMode, BasicBaseLine));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - d->_shadowBorderWidth, foregroundRect.width(), height() - d->_shadowBorderWidth);
    }
    //文字绘制
    //painter.setPen(isEnabled() ? d->_themeMode == ElaThemeType::Light ? d->_lightTextColor : d->_darkTextColor : ElaThemeColor(d->_themeMode, BasicTextDisable));
    //painter.drawText(foregroundRect, Qt::AlignCenter, text());
    painter.restore();
}

void ElaLoadButton::setText(const QString &text)
{
    Q_D(ElaLoadButton);
    d->_text->setText(text);
    QPushButton::setText(text);
}

void ElaLoadButton::setEnabled(bool enabled)
{
    Q_D(ElaLoadButton);
    QColor textColor = (enabled ? d->_themeMode == ElaThemeType::Light ? d->_lightTextColor : d->_darkTextColor : ElaThemeColor(d->_themeMode, BasicTextDisable));
    d->_text->setStyleSheet(QString("QLabel{color:%1}").arg(textColor.name()));
    QPushButton::setEnabled(enabled);
}

void ElaLoadButton::setLoading(bool loading, int waittime)
{
    Q_D(ElaLoadButton);
    setEnabled(!loading);
    if (loading) {
        d->_progressRing->start();
        d->_progressRing->setCustomBackgroundColor(QColor("#D6D6D6"), QColor("#4E4E4E"));
        d->_outTimer->start(waittime);
    } else {
        d->_progressRing->stop();
        d->_progressRing->setCustomBackgroundColor(Qt::transparent, Qt::transparent);
        d->_outTimer->stop();
    }
}
