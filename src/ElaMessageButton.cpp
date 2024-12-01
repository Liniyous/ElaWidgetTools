#include "ElaMessageButton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaMessageBar.h"
#include "ElaTheme.h"
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
    font.setPixelSize(15);
    setFont(font);
    setText("Message");
    setObjectName("ElaMessageButton");
    setStyleSheet("#ElaMessageButton{background-color:transparent;}");
    d->_pDisplayMsec = 2000;
    d->_pMessageMode = ElaMessageBarType::Success;
    d->_pPositionPolicy = ElaMessageBarType::TopRight;
    d->_themeMode = eTheme->getThemeMode();
    d->_pMessageTargetWidget = nullptr;
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
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
    // eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    // 背景绘制
    painter.save();
    QRect foregroundRect(d->_penBorderWidth + d->_shadowBorderWidth, d->_penBorderWidth + d->_shadowBorderWidth, width() - 2 * (d->_penBorderWidth + d->_shadowBorderWidth), height() - 2 * (d->_penBorderWidth + d->_shadowBorderWidth));
    painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), d->_penBorderWidth));
    painter.setBrush(isEnabled() ? underMouse() ? ElaThemeColor(d->_themeMode, BasicHover) : ElaThemeColor(d->_themeMode, BasicBase) : ElaThemeColor(d->_themeMode, BasicDisable));
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

    //文字绘制
    painter.setPen(isEnabled() ? d->_isLeftButtonPress ? ElaThemeColor(d->_themeMode, BasicTextPress) : ElaThemeColor(d->_themeMode, BasicText) : ElaThemeColor(d->_themeMode, BasicTextDisable));
    painter.drawText(rect(), Qt::AlignCenter, text());
    painter.restore();
}
