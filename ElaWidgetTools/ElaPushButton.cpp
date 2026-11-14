#include "ElaPushButton.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaControlStyle.h"
#include "ElaTheme.h"
#include "private/ElaPushButtonPrivate.h"

#include <QPainter>
#include <QStyleOption>

ELA_PROPERTY_Q_CPP(ElaPushButton, int, BorderRadius)
ELA_PROPERTY_Q_CPP(ElaPushButton, QColor, LightDefaultColor)
ELA_PROPERTY_Q_CPP(ElaPushButton, QColor, DarkDefaultColor)
ELA_PROPERTY_Q_CPP(ElaPushButton, QColor, LightHoverColor)
ELA_PROPERTY_Q_CPP(ElaPushButton, QColor, DarkHoverColor)
ELA_PROPERTY_Q_CPP(ElaPushButton, QColor, LightPressColor)
ELA_PROPERTY_Q_CPP(ElaPushButton, QColor, DarkPressColor)
ElaPushButton::ElaPushButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaPushButtonPrivate())
{
    Q_D(ElaPushButton);
    d->q_ptr = this;
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
    setFixedHeight(30);
    QFont font = this->font();
    font.setPixelSize(eApp->getFontPixelSize() + 2);
    setFont(font);
    setObjectName("ElaPushButton");
    setStyleSheet("#ElaPushButton{background-color:transparent;}");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
}

ElaPushButton::ElaPushButton(const QString& text, QWidget* parent)
    : ElaPushButton(parent)
{
    setText(text);
}

ElaPushButton::~ElaPushButton()
{
}

void ElaPushButton::setIsAccent(bool isAccent)
{
    Q_D(ElaPushButton);
    d->_isAccent = isAccent;
    update();
}

bool ElaPushButton::getIsAccent() const
{
    Q_D(const ElaPushButton);
    return d->_isAccent;
}

void ElaPushButton::setLightTextColor(const QColor& color)
{
    Q_D(ElaPushButton);
    d->_lightTextColor = color;
}

const QColor& ElaPushButton::getLightTextColor() const
{
    Q_D(const ElaPushButton);
    return d->_lightTextColor;
}

void ElaPushButton::setDarkTextColor(const QColor& color)
{
    Q_D(ElaPushButton);
    d->_darkTextColor = color;
}

const QColor& ElaPushButton::getDarkTextColor() const
{
    Q_D(const ElaPushButton);
    return d->_darkTextColor;
}

QSize ElaPushButton::sizeHint() const
{
    const auto hint = QPushButton::sizeHint();
    const auto metrics = fontMetrics();
    const int margin = qMax(12, metrics.height() * 3 / 4);
    int contentWidth = metrics.horizontalAdvance(text());
    if (!icon().isNull())
    {
        contentWidth += iconSize().width() + (text().isEmpty() ? 0 : qMax(6, metrics.horizontalAdvance(' ')));
    }
    return {qMax(hint.width(), contentWidth + margin * 2), hint.height()};
}

QSize ElaPushButton::minimumSizeHint() const
{
    return sizeHint();
}

void ElaPushButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaPushButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

    // 背景和边框共用圆角轮廓
    const auto foregroundRect = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    painter.setPen(QPen(ElaControlStyle::getControlBorder(foregroundRect, d->_themeMode, isEnabled() && !isDown()), 1));
    if (d->_isAccent)
    {
        QStyleOption option;
        option.initFrom(this);
        option.state.setFlag(QStyle::State_Sunken, isDown());
        painter.setBrush(ElaColorAnimation::getColor(this, ElaControlStyle::getControlBackgroundColor(d->_themeMode, option.state, true)));
    }
    else if (d->_themeMode == ElaThemeType::Light)
    {
        painter.setBrush(ElaColorAnimation::getColor(this, isEnabled() ? isDown() ? d->_pLightPressColor : (underMouse() ? d->_pLightHoverColor : d->_pLightDefaultColor) : ElaThemeColor(d->_themeMode, BasicDisable), "Background"));
    }
    else
    {
        painter.setBrush(ElaColorAnimation::getColor(this, isEnabled() ? isDown() ? d->_pDarkPressColor : (underMouse() ? d->_pDarkHoverColor : d->_pDarkDefaultColor) : ElaThemeColor(d->_themeMode, BasicDisable), "Background"));
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 图标和文字整体居中
    painter.setPen(!isEnabled() ? ElaThemeColor(d->_themeMode, BasicTextDisable) : d->_isAccent ? ElaThemeColor(d->_themeMode, BasicTextInvert)
                       : d->_themeMode == ElaThemeType::Light                                   ? d->_lightTextColor
                                                                                                : d->_darkTextColor);
    QRect textRect = rect();
    if (!icon().isNull())
    {
        const int spacing = text().isEmpty() ? 0 : qMax(6, fontMetrics().horizontalAdvance(' '));
        const int textWidth = fontMetrics().horizontalAdvance(text());
        const QSize size = iconSize();
        const QRect iconRect((width() - size.width() - spacing - textWidth) / 2, (height() - size.height()) / 2, size.width(), size.height());
        icon().paint(&painter, iconRect, Qt::AlignCenter, isEnabled() ? QIcon::Normal : QIcon::Disabled);
        textRect.setLeft(iconRect.right() + spacing + 1);
        textRect.setWidth(textWidth);
    }
    painter.drawText(textRect, Qt::AlignCenter, text());
}
