#include "ElaInteractiveCard.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaInteractiveCardPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, QString, Title);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, QString, SubTitle);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, TitlePixelSize);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, SubTitlePixelSize);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, TitleSpacing);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, QPixmap, CardPixmap);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, QSize, CardPixmapSize);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, CardPixmapBorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, ElaCardPixType::PixMode, CardPixMode);
ElaInteractiveCard::ElaInteractiveCard(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaInteractiveCardPrivate())
{
    _statusXOffset = 0;
    _timeContent = "";
    _statusContent = "";

    Q_D(ElaInteractiveCard);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    d->_pTitlePixelSize = 15;
    d->_pSubTitlePixelSize = 12;
    setMinimumSize(270, 80);
    d->_pCardPixmapSize = QSize(64, 64);
    d->_pTitleSpacing = 2;
    d->_pCardPixmapBorderRadius = 6;
    d->_pCardPixMode = ElaCardPixType::PixMode::Ellipse;
    d->_themeMode = eTheme->getThemeMode();
    setMouseTracking(true);
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaInteractiveCard::~ElaInteractiveCard()
{
}

void ElaInteractiveCard::setCardPixmapSize(int width, int height)
{
    Q_D(ElaInteractiveCard);
    d->_pCardPixmapSize = QSize(width, height);
    Q_EMIT pCardPixmapSizeChanged();
}

void ElaInteractiveCard::paintEvent(QPaintEvent* event)
{
    Q_D(ElaInteractiveCard);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(underMouse() ? ElaThemeColor(d->_themeMode, BasicHoverAlpha) : Qt::transparent);
    painter.drawRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
    // 图片绘制
    if (!d->_pCardPixmap.isNull())
    {
        painter.save();
        QPainterPath path;
        if (d->_pCardPixMode == ElaCardPixType::PixMode::Ellipse)
        {
            path.addEllipse(QPointF(d->_pCardPixmapSize.width() / 2 + 10, height() / 2), d->_pCardPixmapSize.width() / 2, d->_pCardPixmapSize.height() / 2);
            painter.setClipPath(path);
            painter.drawPixmap(QRect(10, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height()), d->_pCardPixmap); // rect为绘制区域，image为要绘制的图片
        }
        else if (d->_pCardPixMode == ElaCardPixType::PixMode::Default)
        {
            painter.drawPixmap(10, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height(), d->_pCardPixmap);
        }
        else if (d->_pCardPixMode == ElaCardPixType::PixMode::RoundedRect)
        {
            path.addRoundedRect(QRectF(10, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height()), d->_pCardPixmapBorderRadius, d->_pCardPixmapBorderRadius);
            painter.setClipPath(path);
            painter.drawPixmap(10, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height(), d->_pCardPixmap);
        }
        painter.restore();
    }
    // 文字绘制
    painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
    QFont font = this->font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(d->_pTitlePixelSize);
    painter.setFont(font);
    int textStartX = d->_pCardPixmapSize.width() + 26;
    int textWidth = static_cast<int>((width() - textStartX) * 0.75) - 20;
    painter.drawText(QRect(textStartX, rect().y(), textWidth, height() / 2 - d->_pTitleSpacing), Qt::TextWordWrap | Qt::AlignBottom | Qt::AlignLeft, d->_pTitle);
    font.setWeight(QFont::Normal);
    font.setPixelSize(d->_pSubTitlePixelSize);
    painter.setFont(font);
    painter.drawText(QRect(textStartX, height() / 2 + d->_pTitleSpacing, textWidth, height() / 2 - d->_pTitleSpacing), Qt::TextWordWrap | Qt::AlignTop | Qt::AlignLeft, d->_pSubTitle);

    // fuqaq : draw time and status content
    if (_timeContent != ""){
        painter.setPen(_timeContentColor);
        painter.setFont(_timeContentFont);
        int exContentStartX = textStartX + textWidth;
        int exContentWidth  = static_cast<int>((width() - textStartX) * 0.25) + 20;
        painter.drawText(QRect(exContentStartX, rect().y(), exContentWidth, height() / 2 - d->_pTitleSpacing),
                         Qt::TextWordWrap | Qt::AlignCenter | Qt::AlignLeft, _timeContent);

        if (!_statusChecked) {
            // 绘制状态内容（红色圆形 + 白色数字）
            painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
            int circleDiameter = height() / 4;
            int circleX = exContentStartX + _statusXOffset;
            int circleY = static_cast<int>(height() * 0.625);

            // 绘制红色圆形
            painter.setBrush(Qt::red);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(circleX, circleY, circleDiameter, circleDiameter);

            painter.setPen(Qt::white);
            painter.setFont(_statusContentFont);

            QRect textRect(circleX, circleY, circleDiameter, circleDiameter);
            painter.drawText(textRect, Qt::AlignCenter, _statusContent);
        }
    }
    painter.restore();
}

void ElaInteractiveCard::setTimeContent(const QString& content,const QColor& color,const QFont& font) {
    _timeContent = content;
    _timeContentColor = color;
    _timeContentFont = font;
    update();
}

void ElaInteractiveCard::setStatusContent(const QString& content,const QFont& font,int xOffset) {
    _statusXOffset = xOffset;
    _statusContent = content;
    _statusContentFont = font;
    _statusChecked = false;
    update(static_cast<int>(width() * 0.75) + 35,static_cast<int>(height() * 0.5),static_cast<int>(width() * 0.25),static_cast<int>(height() * 0.5));
}

void ElaInteractiveCard::changeStatus(bool _curStatus) {
    _statusChecked = _curStatus;
    update(static_cast<int>(width() * 0.75) + 35,static_cast<int>(height() * 0.5),static_cast<int>(width() * 0.25),static_cast<int>(height() * 0.5));
}

