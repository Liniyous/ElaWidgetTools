#include "ElaIconButton.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaTheme.h"
#include "private/ElaIconButtonPrivate.h"

#include <QEvent>
#include <QFont>
#include <QPainter>
#include <QPainterPath>

ELA_PROPERTY_Q_CPP(ElaIconButton, int, BorderRadius)
ELA_PROPERTY_Q_CPP(ElaIconButton, qreal, Opacity);
ELA_PROPERTY_Q_CPP(ElaIconButton, QColor, LightHoverColor);
ELA_PROPERTY_Q_CPP(ElaIconButton, QColor, DarkHoverColor);
ELA_PROPERTY_Q_CPP(ElaIconButton, QColor, LightIconColor);
ELA_PROPERTY_Q_CPP(ElaIconButton, QColor, DarkIconColor);
ELA_PROPERTY_Q_CPP(ElaIconButton, QColor, LightHoverIconColor);
ELA_PROPERTY_Q_CPP(ElaIconButton, QColor, DarkHoverIconColor);
ELA_PROPERTY_Q_CPP(ElaIconButton, bool, IsSelected);
ElaIconButton::ElaIconButton(const QPixmap& pix, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_iconPix = pix.copy();
    d->_pOpacity = 1;
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, BasicHoverAlpha);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, BasicHoverAlpha);
    d->_pLightIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pLightHoverIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkHoverIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = eTheme->getThemeMode();
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]() {
        update();
    });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaIconButton::ElaIconButton(ElaIconType::IconName awesome, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_pOpacity = 1;
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, BasicHoverAlpha);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, BasicHoverAlpha);
    d->_pLightIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pLightHoverIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkHoverIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = eTheme->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(eApp->getFontPixelSize() + 2);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar(awesome));
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]() {
        update();
    });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaIconButton::ElaIconButton(ElaIconType::IconName awesome, int pixelSize, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_pOpacity = 1;
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, BasicHoverAlpha);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, BasicHoverAlpha);
    d->_pLightIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pLightHoverIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkHoverIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = eTheme->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(pixelSize);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar(awesome));
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]() {
        update();
    });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaIconButton::ElaIconButton(ElaIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_pOpacity = 1;
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, BasicHoverAlpha);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, BasicHoverAlpha);
    d->_pLightIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pLightHoverIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkHoverIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = eTheme->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(pixelSize);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar(awesome));
    this->setFixedSize(fixedWidth, fixedHeight);
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]() {
        update();
    });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaIconButton::~ElaIconButton()
{
}

void ElaIconButton::setAwesome(ElaIconType::IconName awesome)
{
    Q_D(ElaIconButton);
    d->_pAwesome = awesome;
    this->setText(QChar(awesome));
}

ElaIconType::IconName ElaIconButton::getAwesome() const
{
    return this->d_ptr->_pAwesome;
}

void ElaIconButton::setPixmap(const QPixmap& pix)
{
    Q_D(ElaIconButton);
    d->_iconPix = pix.copy();
}

void ElaIconButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaIconButton);
    QPainter painter(this);
    painter.save();
    painter.setOpacity(d->_pOpacity);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    const QColor background = d->_pIsSelected || (isEnabled() && underMouse())
        ? (d->_themeMode == ElaThemeType::Light ? d->_pLightHoverColor : d->_pDarkHoverColor)
        : QColor(Qt::transparent);
    painter.setBrush(ElaColorAnimation::getColor(this, background));
    painter.drawRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
    // 图标绘制
    if (!d->_iconPix.isNull())
    {
        QPainterPath path;
        path.addEllipse(rect());
        painter.setClipPath(path);
        painter.drawPixmap(rect(), d->_iconPix);
    }
    else
    {
        painter.setPen(isEnabled() ? d->_themeMode == ElaThemeType::Light ? underMouse() ? d->_pLightHoverIconColor : d->_pLightIconColor : underMouse() ? d->_pDarkHoverIconColor
                                                                                                                                                         : d->_pDarkIconColor
                                   : ElaThemeColor(d->_themeMode, BasicTextDisable));
        painter.drawText(rect(), Qt::AlignCenter, QChar(d->_pAwesome));
    }
    painter.restore();
}
