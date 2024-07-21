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
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, QWidget*, MessageTargetWidget)
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, ElaMessageBarType::MessageMode, MessageMode);
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, ElaMessageBarType::PositionPolicy, PositionPolicy);

ElaMessageButton::ElaMessageButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaMessageButtonPrivate())
{
    Q_D(ElaMessageButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    setMouseTracking(true);
    setFixedSize(80, 38);
    QFont font = this->font();
    font.setPointSize(11);
    setFont(font);
    setText("Message");
    setObjectName("ElaMessageButton");
    setStyleSheet("#ElaMessageButton{background-color:transparent;}");
    d->_pDisplayMsec = 2000;
    d->_pMessageMode = ElaMessageBarType::Success;
    d->_pPositionPolicy = ElaMessageBarType::TopRight;
    d->_themeMode = eApp->getThemeMode();
    d->_pMessageTargetWidget = nullptr;
    connect(eApp, &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
    connect(this, &ElaMessageButton::clicked, this, [=]() {
        switch(d->_pMessageMode)
        {
        case ElaMessageBarType::Success:
        {
            ElaMessageBar::success(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec,d->_pMessageTargetWidget);
            break;
        }
        case ElaMessageBarType::Warning:
        {
            ElaMessageBar::warning(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec,d->_pMessageTargetWidget);
            break;
        }
        case ElaMessageBarType::Information:
        {
            ElaMessageBar::information(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec,d->_pMessageTargetWidget);
            break;
        }
        case ElaMessageBarType::Error:
        {
            ElaMessageBar::error(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec,d->_pMessageTargetWidget);
            break;
        }
        } });
}

ElaMessageButton::ElaMessageButton(QString text, QWidget* parent)
    : ElaMessageButton(parent)
{
    setText(text);
}

ElaMessageButton::~ElaMessageButton()
{
}

void ElaMessageButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaMessageButton);
    if (event->button() == Qt::LeftButton)
    {
        d->_isLeftButtonPress = true;
    }
    QPushButton::mousePressEvent(event);
}

void ElaMessageButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaMessageButton);
    d->_isLeftButtonPress = false;
    QPushButton::mouseReleaseEvent(event);
}

void ElaMessageButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaMessageButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    eApp->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

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

    //文字绘制
    painter.setPen(d->_themeMode == ElaApplicationType::Light ? (d->_isLeftButtonPress ? QColor(0x64, 0x66, 0x73) : Qt::black) : (d->_isLeftButtonPress ? QColor(0xA1, 0xA2, 0xA2) : Qt::white));
    painter.drawText(rect(), Qt::AlignCenter, text());
    painter.restore();
}
