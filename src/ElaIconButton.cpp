#include "ElaIconButton.h"

#include <QFont>
#include <QPainter>

#include "ElaApplication.h"
#include "private/ElaIconButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, LightHoverColor);
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, DarkHoverColor);
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, LightIconColor);
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, DarkIconColor);
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, bool, IsSelected);
ElaIconButton::ElaIconButton(ElaIconType awesome, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_pLightHoverColor = QColor(0xEA, 0xE9, 0xF0);
    d->_pDarkHoverColor = QColor(0x2F, 0x2E, 0x2C);
    d->_pLightIconColor = Qt::black;
    d->_pDarkIconColor = Qt::white;
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(15);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]()
            { update(); });
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode)
            { d->_themeMode = themeMode; });
}

ElaIconButton::ElaIconButton(ElaIconType awesome, int pixelSize, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_pLightHoverColor = QColor(0xEA, 0xE9, 0xF0);
    d->_pDarkHoverColor = QColor(0x2F, 0x2E, 0x2C);
    d->_pLightIconColor = Qt::black;
    d->_pDarkIconColor = Qt::white;
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(pixelSize);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]()
            { update(); });
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode)
            { d->_themeMode = themeMode; });
}

ElaIconButton::ElaIconButton(ElaIconType awesome, int pixelSize, int fixedWidth, int fixedHeight, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_pLightHoverColor = QColor(0xEA, 0xE9, 0xF0);
    d->_pDarkHoverColor = QColor(0x2F, 0x2E, 0x2C);
    d->_pLightIconColor = Qt::black;
    d->_pDarkIconColor = Qt::white;
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(pixelSize);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
    this->setFixedSize(fixedWidth, fixedHeight);
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]()
            { update(); });
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode)
            { d->_themeMode = themeMode; });
}

ElaIconButton::~ElaIconButton()
{
}

void ElaIconButton::setAwesome(ElaIconType awesome)
{
    Q_D(ElaIconButton);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
}

ElaIconType ElaIconButton::getAwesome() const
{
    return this->d_ptr->_pAwesome;
}

void ElaIconButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaIconButton);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(d->_pIsSelected ? d->_themeMode == ElaApplicationType::Light ? d->_pLightHoverColor : d->_pDarkHoverColor
                     : isEnabled()   ? underMouse() ? d->_themeMode == ElaApplicationType::Light ? d->_pLightHoverColor : d->_pDarkHoverColor : Qt::transparent
                                     : Qt::transparent);
    painter.drawRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
    // 图标绘制
    painter.setPen(isEnabled() ? d->_themeMode == ElaApplicationType::Light ? d->_pLightIconColor : d->_pDarkIconColor : QColor(0xA1, 0xA1, 0xA4));
    painter.drawText(0, 0, width(), height(), Qt::AlignVCenter | Qt::AlignHCenter, QChar((unsigned short)d->_pAwesome));
    painter.restore();
}
