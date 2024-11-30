#include "ElaAcrylicUrlCard.h"

#include <QDesktopServices>
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
Q_PROPERTY_CREATE_Q_CPP(ElaAcrylicUrlCard, int, SubTitleSpacing);
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
    d->_pSubTitleSpacing = 10;
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
    // 亚克力绘制
    painter.save();
    painter.setPen(QPen(underMouse() ? ElaThemeColor(d->_themeMode, BasicBorderHover) : ElaThemeColor(d->_themeMode, BasicBorder), d->_borderWidth));
    QColor brushColor;
    brushColor = underMouse() ? ElaThemeColor(d->_themeMode, UrlCardHover) : ElaThemeColor(d->_themeMode, UrlCardBase);
    brushColor.setAlpha(d->_pBrushAlpha);
    painter.setBrush(brushColor);
    QRect foregroundRect = rect();
    foregroundRect.adjust(d->_borderWidth, d->_borderWidth, -d->_borderWidth, -d->_borderWidth);
    int width = foregroundRect.width();
    painter.setOpacity(d->_pNoiseOpacity);
    painter.drawPixmap(foregroundRect, d->_noisePix);
    painter.setOpacity(d->_pMainOpacity);
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.restore();

    // 图片绘制
    QRectF pixRect(width / 8.5, height() / 4 - d->_pCardPixmapSize.height() / 2, d->_pCardPixmapSize.width(), d->_pCardPixmapSize.height());
    if (!d->_pCardPixmap.isNull())
    {
        painter.save();
        QPainterPath path;
        if (d->_pCardPixMode == ElaCardPixType::PixMode::Ellipse)
        {
            path.addEllipse(pixRect.center(), d->_pCardPixmapSize.width() / 2, d->_pCardPixmapSize.height() / 2);
            painter.setClipPath(path);
            painter.drawPixmap(pixRect, d->_pCardPixmap, d->_pCardPixmap.rect());
        }
        else if (d->_pCardPixMode == ElaCardPixType::PixMode::Default)
        {
            painter.drawPixmap(pixRect, d->_pCardPixmap, d->_pCardPixmap.rect());
        }
        else if (d->_pCardPixMode == ElaCardPixType::PixMode::RoundedRect)
        {
            path.addRoundedRect(pixRect, d->_pCardPixmapBorderRadius, d->_pCardPixmapBorderRadius);
            painter.setClipPath(path);
            painter.drawPixmap(pixRect, d->_pCardPixmap, d->_pCardPixmap.rect());
        }
        painter.restore();
    }

    // 文字绘制
    painter.save();
    QFont font = this->font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(d->_pTitlePixelSize);
    painter.setFont(font);
    painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
    painter.drawText(QRect(pixRect.x(), pixRect.bottom() + d->_pTitleSpacing, width - width / 7, height() / 3), Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, d->_pTitle);
    font.setWeight(QFont::Normal);
    font.setPixelSize(d->_pSubTitlePixelSize);
    painter.setFont(font);
    painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
    int titleHeight = painter.fontMetrics().boundingRect(d->_pTitle).height() * 1.1;
    painter.drawText(QRect(pixRect.x(), d->_pSubTitleSpacing + titleHeight + pixRect.bottom() + d->_pTitleSpacing, width - width / 7, height() / 3), Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, d->_pSubTitle);
    painter.restore();

    // 图标绘制
    painter.save();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(13);
    painter.setFont(iconFont);
    painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
    painter.drawText(width - 1.5 * iconFont.pixelSize(), height() - iconFont.pixelSize(), QChar((unsigned short)ElaIconType::UpRightFromSquare));
    painter.restore();
}
