#include "ElaIconButton.h"

#include <QFont>
#include <QPainter>
#include <QPropertyAnimation>

#include "ElaTheme.h"
#include "private/ElaIconButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, qreal, Opacity);
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, LightHoverColor);
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, DarkHoverColor);
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, LightIconColor);
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, DarkIconColor);
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, LightHoverIconColor);
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, DarkHoverIconColor);
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, bool, IsSelected);
ElaIconButton::ElaIconButton(ElaIconType::IconName awesome, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_pHoverAlpha = 0;
    d->_pOpacity = 1;
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, IconButtonDefaultHover);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, IconButtonDefaultHover);
    d->_pLightIconColor = ElaThemeColor(ElaThemeType::Light, IconButtonDefaultIconText);
    d->_pDarkIconColor = ElaThemeColor(ElaThemeType::Dark, IconButtonDefaultIconText);
    d->_pLightHoverIconColor = ElaThemeColor(ElaThemeType::Light, IconButtonDefaultIconText);
    d->_pDarkHoverIconColor = ElaThemeColor(ElaThemeType::Dark, IconButtonDefaultIconText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = eTheme->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(15);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]() { update(); });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaIconButton::ElaIconButton(ElaIconType::IconName awesome, int pixelSize, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_pHoverAlpha = 0;
    d->_pOpacity = 1;
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, IconButtonDefaultHover);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, IconButtonDefaultHover);
    d->_pLightIconColor = ElaThemeColor(ElaThemeType::Light, IconButtonDefaultIconText);
    d->_pDarkIconColor = ElaThemeColor(ElaThemeType::Dark, IconButtonDefaultIconText);
    d->_pLightHoverIconColor = ElaThemeColor(ElaThemeType::Light, IconButtonDefaultIconText);
    d->_pDarkHoverIconColor = ElaThemeColor(ElaThemeType::Dark, IconButtonDefaultIconText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = eTheme->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(pixelSize);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]() { update(); });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaIconButton::ElaIconButton(ElaIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_pHoverAlpha = 0;
    d->_pOpacity = 1;
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, IconButtonDefaultHover);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, IconButtonDefaultHover);
    d->_pLightIconColor = ElaThemeColor(ElaThemeType::Light, IconButtonDefaultIconText);
    d->_pDarkIconColor = ElaThemeColor(ElaThemeType::Dark, IconButtonDefaultIconText);
    d->_pLightHoverIconColor = ElaThemeColor(ElaThemeType::Light, IconButtonDefaultIconText);
    d->_pDarkHoverIconColor = ElaThemeColor(ElaThemeType::Dark, IconButtonDefaultIconText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = eTheme->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(pixelSize);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
    this->setFixedSize(fixedWidth, fixedHeight);
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]() { update(); });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaIconButton::~ElaIconButton()
{
}

void ElaIconButton::setAwesome(ElaIconType::IconName awesome)
{
    Q_D(ElaIconButton);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
}

ElaIconType::IconName ElaIconButton::getAwesome() const
{
    return this->d_ptr->_pAwesome;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void ElaIconButton::enterEvent(QEnterEvent* event)
#else
void ElaIconButton::enterEvent(QEvent* event)
#endif
{
    Q_D(ElaIconButton);
    if (isEnabled())
    {
        d->_isAlphaAnimationFinished = false;
        QPropertyAnimation* alphaAnimation = new QPropertyAnimation(d, "pHoverAlpha");
        connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() {
            d->_isAlphaAnimationFinished = true;
        });
        alphaAnimation->setDuration(175);
        alphaAnimation->setStartValue(d->_pHoverAlpha);
        alphaAnimation->setEndValue(255);
        alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QPushButton::enterEvent(event);
}

void ElaIconButton::leaveEvent(QEvent* event)
{
    Q_D(ElaIconButton);
    if (isEnabled())
    {
        d->_isAlphaAnimationFinished = false;
        QPropertyAnimation* alphaAnimation = new QPropertyAnimation(d, "pHoverAlpha");
        connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() {
            d->_isAlphaAnimationFinished = true;
        });
        alphaAnimation->setDuration(175);
        alphaAnimation->setStartValue(d->_pHoverAlpha);
        alphaAnimation->setEndValue(0);
        alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QPushButton::leaveEvent(event);
}

void ElaIconButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaIconButton);
    QPainter painter(this);
    painter.save();
    painter.setOpacity(d->_pOpacity);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    if (d->_isAlphaAnimationFinished || d->_pIsSelected)
    {
        painter.setBrush(d->_pIsSelected ? d->_themeMode == ElaThemeType::Light ? d->_pLightHoverColor : d->_pDarkHoverColor
                         : isEnabled()   ? underMouse() ? d->_themeMode == ElaThemeType::Light ? d->_pLightHoverColor : d->_pDarkHoverColor : Qt::transparent
                                         : Qt::transparent);
    }
    else
    {
        QColor hoverColor = d->_themeMode == ElaThemeType::Light ? d->_pLightHoverColor : d->_pDarkHoverColor;
        hoverColor.setAlpha(d->_pHoverAlpha);
        painter.setBrush(hoverColor);
    }
    painter.drawRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
    // 图标绘制
    painter.setPen(isEnabled() ? d->_themeMode == ElaThemeType::Light ? underMouse() ? d->_pLightHoverIconColor : d->_pLightIconColor : underMouse() ? d->_pDarkHoverIconColor
                                                                                                                                                     : d->_pDarkIconColor
                               : ElaThemeColor(d->_themeMode, WindowTextDisable));
    QColor color = isEnabled() ? d->_themeMode == ElaThemeType::Light ? underMouse() ? d->_pLightHoverIconColor : d->_pLightIconColor : underMouse() ? d->_pDarkHoverIconColor
                                                                                                                                                     : d->_pDarkIconColor
                               : ElaThemeColor(d->_themeMode, WindowTextDisable);
    painter.drawText(0, 0, width(), height(), Qt::AlignVCenter | Qt::AlignHCenter, QChar((unsigned short)d->_pAwesome));
    painter.restore();
}
