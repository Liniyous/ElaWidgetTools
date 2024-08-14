#include "ElaAcrylicUrlCard.h"

#include <QDesktopServices>
#include <QGraphicsBlurEffect>
#include <QPainter>
#include <QPainterPath>
#include <QUrl>

#include "ElaTheme.h"
#include "private/ElaAcrylicUrlCardPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, qreal, MainOpacity)
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, qreal, NoiseOpacity)
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, BrushAlpha)
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, QString, Title);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, QString, SubTitle);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, TitlePixelSize);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, SubTitlePixelSize);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, TitleSpacing);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, QPixmap, CardPixmap);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, QSize, CardPixmapSize);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, CardPixmapBorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, ElaCardPixType::PixMode, CardPixMode);
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, QString, Url);
ElaAcrylicUrlCard::ElaAcrylicUrlCard(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaAcrylicUrlCardPrivate())
{
    Q_D(ElaAcrylicUrlCard);
    d->q_ptr = this;
    d->_noisePix = QPixmap(":/include/Image/noise.png");
    d->_pBorderRadius = 5;
    d->_pMainOpacity = 0.95;
    d->_pNoiseOpacity = 0.06;
    d->_pBrushAlpha = 245;
    d->_pTitlePixelSize = 15;
    d->_pSubTitlePixelSize = 12;
    setFixedSize(180, 200);
    d->_pCardPixmapSize = QSize(54, 54);
    d->_pTitleSpacing = 10;
    d->_pCardPixmapBorderRadius = 6;
    d->_pCardPixMode = ElaCardPixType::PixMode::Ellipse;
    d->_themeMode = eTheme->getThemeMode();
    connect(this, &ElaAcrylicUrlCard::clicked, this, [=]() { QDesktopServices::openUrl(QUrl(d->_pUrl)); });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaAcrylicUrlCard::~ElaAcrylicUrlCard()
{
}

void ElaAcrylicUrlCard::setCardPixmapSize(int width, int height)
{
    Q_D(ElaAcrylicUrlCard);
    d->_pCardPixmapSize = QSize(width, height);
    Q_EMIT pCardPixmapSizeChanged();
}

void ElaAcrylicUrlCard::paintEvent(QPaintEvent* event)
{
    Q_D(ElaAcrylicUrlCard);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    // 亚克力绘制
    painter.save();
    painter.setPen(ElaThemeColor(d->_themeMode, AcrylicUrlCardBorder));
    QColor brushColor;
    brushColor = underMouse() ? ElaThemeColor(d->_themeMode, AcrylicUrlCardHover) : ElaThemeColor(d->_themeMode, AcrylicUrlCardBase);
    brushColor.setAlpha(d->_pBrushAlpha);
    painter.setBrush(brushColor);
    QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * d->_shadowBorderWidth, height() - 2 * d->_shadowBorderWidth);
    int width = foregroundRect.width();
    painter.setClipRect(foregroundRect);
    painter.setOpacity(d->_pNoiseOpacity);
    painter.drawPixmap(foregroundRect, d->_noisePix);
    painter.setOpacity(d->_pMainOpacity);
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.restore();

    // 图片绘制
    QRect pixRect(width / 7, height() / 4 - d->_pCardPixmapSize.height() / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height());
    if (!d->_pCardPixmap.isNull())
    {
        painter.save();
        QPainterPath path;
        if (d->_pCardPixMode == ElaCardPixType::PixMode::Ellipse)
        {
            path.addEllipse(QPointF(d->_pCardPixmapSize.width() / 2 + width / 7, height() / 4), d->_pCardPixmapSize.width() / 2, d->_pCardPixmapSize.height() / 2);
            painter.setClipPath(path);
            painter.drawPixmap(pixRect, d->_pCardPixmap); // rect为绘制区域，image为要绘制的图片
        }
        else if (d->_pCardPixMode == ElaCardPixType::PixMode::Default)
        {
            painter.drawPixmap(pixRect, d->_pCardPixmap);
        }
        else if (d->_pCardPixMode == ElaCardPixType::PixMode::RoundedRect)
        {
            path.addRoundedRect(QRectF(width / 7, (height() - d->_pCardPixmapSize.height()) / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height()), d->_pCardPixmapBorderRadius, d->_pCardPixmapBorderRadius);
            painter.setClipPath(path);
            painter.drawPixmap(pixRect, d->_pCardPixmap);
        }
        painter.restore();
    }

    // 文字绘制
    painter.save();
    painter.setPen(QPen(Qt::black));
    QFont font = this->font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(d->_pTitlePixelSize);
    painter.setFont(font);
    painter.setPen(ElaThemeColor(d->_themeMode, WindowText));
    painter.drawText(QRect(pixRect.x(), pixRect.bottom() + d->_pTitleSpacing, width - width / 7, height() / 3), Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, d->_pTitle);
    font.setWeight(QFont::Normal);
    font.setPixelSize(d->_pSubTitlePixelSize);
    painter.setFont(font);
    painter.setPen(ElaThemeColor(d->_themeMode, AcrylicUrlCardSubTitleText));
    int titleHeight = painter.fontMetrics().boundingRect(d->_pTitle).height() * 1.8;
    painter.drawText(QRect(pixRect.x(), titleHeight + pixRect.bottom() + d->_pTitleSpacing, width - width / 7, height() / 3), Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, d->_pSubTitle);
    painter.restore();

    // 图标绘制
    painter.save();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(13);
    painter.setFont(iconFont);
    painter.setPen(ElaThemeColor(d->_themeMode, WindowText));
    painter.drawText(width - iconFont.pixelSize() - 1, height() - iconFont.pixelSize(), QChar((unsigned short)ElaIconType::UpRightFromSquare));
    painter.restore();
}
