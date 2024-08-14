#include "ElaReminderCard.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaReminderCardPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaReminderCard, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaReminderCard, QString, Title);
Q_PROPERTY_CREATE_Q_CPP(ElaReminderCard, QString, SubTitle);
Q_PROPERTY_CREATE_Q_CPP(ElaReminderCard, int, TitlePixelSize);
Q_PROPERTY_CREATE_Q_CPP(ElaReminderCard, int, SubTitlePixelSize);
Q_PROPERTY_CREATE_Q_CPP(ElaReminderCard, int, TitleSpacing);
Q_PROPERTY_CREATE_Q_CPP(ElaReminderCard, QPixmap, CardPixmap);
Q_PROPERTY_CREATE_Q_CPP(ElaReminderCard, QSize, CardPixmapSize);
Q_PROPERTY_CREATE_Q_CPP(ElaReminderCard, int, CardPixmapBorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaReminderCard, ElaCardPixType::PixMode, CardPixMode);

ElaReminderCard::ElaReminderCard(QWidget* parent)
    : QPushButton{parent}, d_ptr(new ElaReminderCardPrivate())
{
    Q_D(ElaReminderCard);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    d->_pTitlePixelSize = 15;
    d->_pSubTitlePixelSize = 12;
    setFixedSize(330, 105);
    d->_pCardPixmapSize = QSize((height() - d->_shadowBorderWidth) * 0.8, (height() - d->_shadowBorderWidth) * 0.8);
    d->_pTitleSpacing = 3;
    d->_pCardPixmapBorderRadius = 6;
    d->_pCardPixMode = ElaCardPixType::PixMode::Default;
    d->_themeMode = eTheme->getThemeMode();
    setMouseTracking(true);
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaReminderCard::~ElaReminderCard()
{
}

void ElaReminderCard::setCardPixmapSize(int width, int height)
{
    Q_D(ElaReminderCard);
    d->_pCardPixmapSize = QSize(width, height);
    Q_EMIT pCardPixmapSizeChanged();
}

void ElaReminderCard::paintEvent(QPaintEvent* event)
{
    Q_D(ElaReminderCard);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    // 背景绘制
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(ElaThemeColor(d->_themeMode, ReminderCardBorder));
    painter.setBrush(underMouse() ? ElaThemeColor(d->_themeMode, ReminderCardHover) : ElaThemeColor(d->_themeMode, ReminderCardBase));
    QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * d->_shadowBorderWidth, height() - 2 * d->_shadowBorderWidth);
    int width = foregroundRect.width();
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

    // 图片绘制
    if (!d->_pCardPixmap.isNull())
    {
        painter.save();
        QPainterPath path;
        if (d->_pCardPixMode == ElaCardPixType::PixMode::Ellipse)
        {
            path.addEllipse(QPointF(d->_pCardPixmapSize.width() / 2 + width / 28, height() / 2), d->_pCardPixmapSize.width() / 2, d->_pCardPixmapSize.height() / 2);
            painter.setClipPath(path);
            painter.drawPixmap(QRect(width / 28 + d->_shadowBorderWidth, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height()), d->_pCardPixmap); // rect为绘制区域，image为要绘制的图片
        }
        else if (d->_pCardPixMode == ElaCardPixType::PixMode::Default)
        {
            painter.drawPixmap(width / 28 + d->_shadowBorderWidth, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height(), d->_pCardPixmap);
        }
        else if (d->_pCardPixMode == ElaCardPixType::PixMode::RoundedRect)
        {
            path.addRoundedRect(QRectF(width / 28, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height()), d->_pCardPixmapBorderRadius, d->_pCardPixmapBorderRadius);
            painter.setClipPath(path);
            painter.drawPixmap(width / 28 + d->_shadowBorderWidth, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height(), d->_pCardPixmap);
        }
        painter.restore();
    }

    // 焦点圆型绘制
    painter.save();
    painter.setBrush(ElaThemeColor(d->_themeMode, ReminderCardMark));
    painter.drawEllipse(QPointF(width * 0.95 + d->_shadowBorderWidth, height() * 0.25), height() / 17, height() / 17);
    painter.restore();

    // 文字绘制
    QFont font = this->font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(d->_pTitlePixelSize);
    painter.setPen(ElaThemeColor(d->_themeMode, WindowText));
    painter.setFont(font);
    // 计算换行 width() / 28 图片左间距 文字右边距  width() / 11 文字左间距
    painter.drawText(d->_pCardPixmapSize.width() + width / 11 + d->_shadowBorderWidth, this->height() / 2 - d->_pTitleSpacing, d->_pTitle);
    font.setWeight(QFont::Normal);
    font.setPixelSize(d->_pSubTitlePixelSize);
    painter.setFont(font);
    painter.setPen(ElaThemeColor(d->_themeMode, ReminderCardSubTitleText));
    qreal rowTextWidth = (this->width() - this->width() / 11 - this->width() / 28 - d->_pCardPixmapSize.width() - 2 * d->_shadowBorderWidth);
    painter.drawText(QRect(d->_pCardPixmapSize.width() + width / 11 + d->_shadowBorderWidth, height() / 2 + d->_pTitleSpacing, rowTextWidth, height() / 2), Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap | Qt::TextWrapAnywhere, d->_pSubTitle);
    painter.restore();
}
