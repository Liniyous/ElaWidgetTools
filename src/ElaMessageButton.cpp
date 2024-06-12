#include "ElaMessageButton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaApplication.h"
#include "ElaMessageBar.h"
#include "private/ElaMessageButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, QString, BarTitle);
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, QString, BarText);
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, int, DisplayMsec);
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, ElaMessageBarType::MessageMode, MessageMode);
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, ElaMessageBarType::PositionPolicy, PositionPolicy);

ElaMessageButton::ElaMessageButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaMessageButtonPrivate())
{
    Q_D(ElaMessageButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    setFixedSize(80, 38);
    QFont font = this->font();
    font.setPointSize(11);
    setFont(font);
    setMouseTracking(true);
    setText("Message");
    setObjectName("ElaMessageButton");
    setStyleSheet("#ElaMessageButton{background-color:transparent;}");
    d->_pDisplayMsec = 2000;
    d->_pMessageMode = ElaMessageBarType::Success;
    d->_pPositionPolicy = ElaMessageBarType::TopRight;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, d, &ElaMessageButtonPrivate::onThemeChanged);
    connect(this, &ElaMessageButton::clicked, this, [=]() {
        switch(d->_pMessageMode)
        {
        case ElaMessageBarType::Success:
        {
            ElaMessageBar::success(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec,window());
            break;
        }
        case ElaMessageBarType::Warning:
        {
            ElaMessageBar::warning(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec,window());
            break;
        }
        case ElaMessageBarType::Information:
        {
            ElaMessageBar::information(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec,window());
            break;
        }
        case ElaMessageBarType::Error:
        {
            ElaMessageBar::error(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec,window());
            break;
        }
        } });
}

ElaMessageButton::ElaMessageButton(QString text, QWidget* parent)
    : QPushButton(text, parent), d_ptr(new ElaMessageButtonPrivate())
{
    Q_D(ElaMessageButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    setMouseTracking(true);
    setFixedSize(80, 38);
    QFont font = this->font();
    font.setPointSize(11);
    setFont(font);
    setObjectName("ElaMessageButton");
    setStyleSheet("#ElaMessageButton{background-color:transparent;}");
    d->_pDisplayMsec = 2000;
    d->_pMessageMode = ElaMessageBarType::Success;
    d->_pPositionPolicy = ElaMessageBarType::TopRight;
    d->_themeMode = ElaApplication::getInstance()->getThemeMode();
    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, d, &ElaMessageButtonPrivate::onThemeChanged);
    connect(this, &ElaMessageButton::clicked, this, [=]() {
                switch(d->_pMessageMode)
                {
                case ElaMessageBarType::Success:
                {
                    ElaMessageBar::success(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec);
                    break;
                }
                case ElaMessageBarType::Warning:
                {
                    ElaMessageBar::warning(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec);
                    break;
                }
                case ElaMessageBarType::Information:
                {
                    ElaMessageBar::information(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec);
                    break;
                }
                case ElaMessageBarType::Error:
                {
                    ElaMessageBar::error(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec);
                    break;
                }
                } });
}

ElaMessageButton::~ElaMessageButton()
{
}

void ElaMessageButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaMessageButton);
    if (event->button() == Qt::LeftButton)
    {
        QPalette palette;
        if (d->_themeMode == ElaApplicationType::Light)
        {
            palette.setColor(QPalette::ButtonText, QColor(0x64, 0x66, 0x73));
        }
        else
        {
            palette.setColor(QPalette::ButtonText, QColor(0xA1, 0xA2, 0xA2));
        }
        setPalette(palette);
    }
    QPushButton::mousePressEvent(event);
}

void ElaMessageButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaMessageButton);
    if (event->button() == Qt::LeftButton)
    {
        QPalette palette;
        if (d->_themeMode == ElaApplicationType::Light)
        {
            palette.setColor(QPalette::ButtonText, Qt::black);
        }
        else
        {
            palette.setColor(QPalette::ButtonText, QColor(0xFE, 0xFE, 0xFE));
        }
        setPalette(palette);
    }
    QPushButton::mouseReleaseEvent(event);
}

void ElaMessageButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaMessageButton);
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
    QRect foregroundRect(d->_penBorderWidth + d->_shadowBorderWidth, d->_penBorderWidth + d->_shadowBorderWidth, width() - 2 * (d->_penBorderWidth + d->_shadowBorderWidth), height() - 2 * (d->_penBorderWidth + d->_shadowBorderWidth));
    if (d->_themeMode == ElaApplicationType::Light)
    {
        painter.setPen(QPen(QColor(0xDF, 0xDF, 0xDF), d->_penBorderWidth));
        painter.setBrush((underMouse() ? QColor(0xF6, 0xF6, 0xF6) : QColor(0xFD, 0xFD, 0xFD)));
    }
    else
    {
        painter.setPen(QPen(QColor(0x50, 0x50, 0x50), d->_penBorderWidth));
        painter.setBrush((underMouse() ? QColor(0x44, 0x44, 0x44) : QColor(0x3E, 0x3E, 0x3E)));
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.restore();
    QPushButton::paintEvent(event);
}
