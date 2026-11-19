#include "ElaToggleButton.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaControlStyle.h"
#include "ElaTheme.h"
#include "private/ElaToggleButtonPrivate.h"

#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

ELA_PROPERTY_Q_CPP(ElaToggleButton, int, BorderRadius)
ELA_PROPERTY_Q_CPP(ElaToggleButton, QString, Text)
ElaToggleButton::ElaToggleButton(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaToggleButtonPrivate())
{
    Q_D(ElaToggleButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = eTheme->getThemeMode();
    setMouseTracking(true);
    setFixedSize(80, ElaWidgetToolsSize::DefaultControlHeight);
    QFont font = this->font();
    font.setPixelSize(eApp->getFontPixelSize() + 2);
    setFont(font);
    setObjectName("ElaToggleButton");
    setStyleSheet("#ElaToggleButton{background-color:transparent;}");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaToggleButton::ElaToggleButton(const QString& text, QWidget* parent)
    : ElaToggleButton(parent)
{
    Q_D(ElaToggleButton);
    d->_pText = text;
}

ElaToggleButton::~ElaToggleButton()
{
}

void ElaToggleButton::setIsToggled(bool isToggled)
{
    Q_D(ElaToggleButton);
    d->_isToggled = isToggled;
    update();
    Q_EMIT toggled(isToggled);
}

bool ElaToggleButton::getIsToggled() const
{
    Q_D(const ElaToggleButton);
    return d->_isToggled;
}

void ElaToggleButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaToggleButton);
    d->_isPressed = true;
    update();
    QWidget::mousePressEvent(event);
}

void ElaToggleButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaToggleButton);
    d->_isPressed = false;
    d->_isToggled = !d->_isToggled;
    update();
    Q_EMIT this->toggled(d->_isToggled);
    QWidget::mouseReleaseEvent(event);
}

void ElaToggleButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaToggleButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter.save();
    QRect foregroundRect(1, 1, width() - 2, height() - 2);
    QStyleOption option;
    option.initFrom(this);
    option.state.setFlag(QStyle::State_Sunken, d->_isPressed);
    const QColor background = ElaControlStyle::getControlBackgroundColor(d->_themeMode, option.state, d->_isToggled);
    painter.setPen(QPen(ElaControlStyle::getControlBorder(foregroundRect, d->_themeMode, isEnabled() && !d->_isPressed), 1));
    painter.setBrush(ElaColorAnimation::getColor(this, background));
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    //文字绘制
    painter.setPen(isEnabled() ? d->_isToggled ? ElaThemeColor(d->_themeMode, BasicTextInvert) : ElaThemeColor(d->_themeMode, BasicText) : ElaThemeColor(d->_themeMode, BasicTextDisable));
    painter.drawText(foregroundRect, Qt::AlignCenter, d->_pText);
    painter.restore();
}
