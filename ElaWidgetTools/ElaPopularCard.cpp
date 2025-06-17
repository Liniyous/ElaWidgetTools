#include "ElaPopularCard.h"

#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTimer>

#include "ElaPopularCardFloater.h"
#include "ElaPopularCardPrivate.h"
#include "ElaPushButton.h"
#include "ElaTheme.h"
Q_PROPERTY_CREATE_Q_CPP(ElaPopularCard, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaPopularCard, QPixmap, CardPixmap)
Q_PROPERTY_CREATE_Q_CPP(ElaPopularCard, QString, Title)
Q_PROPERTY_CREATE_Q_CPP(ElaPopularCard, QString, SubTitle)
Q_PROPERTY_CREATE_Q_CPP(ElaPopularCard, QString, InteractiveTips)
Q_PROPERTY_CREATE_Q_CPP(ElaPopularCard, QString, DetailedText)
Q_PROPERTY_CREATE_Q_CPP(ElaPopularCard, QPixmap, CardFloatPixmap)
ElaPopularCard::ElaPopularCard(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaPopularCardPrivate())
{
    Q_D(ElaPopularCard);
    setFixedSize(320, 120);
    d->q_ptr = this;
    d->_pBorderRadius = 8;
    d->_pHoverYOffset = 0;
    d->_pHoverOpacity = 0;
    setObjectName("ElaPopularCard");
    setStyleSheet("#ElaPopularCard{background-color:transparent}");
    setMouseTracking(true);

    d->_pCardFloatArea = parentWidget();
    d->_floater = new ElaPopularCardFloater(this, d, d->_pCardFloatArea);
    d->_floatTimer = new QTimer(this);
    connect(d->_floatTimer, &QTimer::timeout, d, &ElaPopularCardPrivate::_showFloater);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaPopularCard::~ElaPopularCard()
{
}

void ElaPopularCard::setCardButtonText(QString cardButtonText)
{
    Q_D(ElaPopularCard);
    if (cardButtonText.isEmpty())
    {
        return;
    }
    d->_pCardButtonText = cardButtonText;
    d->_floater->_overButton->setText(d->_pCardButtonText);
    Q_EMIT pCardButtonTextChanged();
}

QString ElaPopularCard::getCardButtonText() const
{
    Q_D(const ElaPopularCard);
    return d->_pCardButtonText;
}

void ElaPopularCard::setCardFloatArea(QWidget* floatArea)
{
    Q_D(ElaPopularCard);
    if (!floatArea || floatArea == this)
    {
        return;
    }
    d->_pCardFloatArea = floatArea;
    d->_floater->setParent(floatArea);
    Q_EMIT pCardFloatAreaChanged();
}

QWidget* ElaPopularCard::getCardFloatArea() const
{
    Q_D(const ElaPopularCard);
    return d->_pCardFloatArea;
}

bool ElaPopularCard::event(QEvent* event)
{
    Q_D(ElaPopularCard);
    switch (event->type())
    {
    case QEvent::Enter:
    {
        d->_floatTimer->start(450);
        QPropertyAnimation* hoverAnimation = new QPropertyAnimation(d, "pHoverYOffset");
        connect(hoverAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            update();
        });
        hoverAnimation->setDuration(130);
        hoverAnimation->setStartValue(d->_pHoverYOffset);
        hoverAnimation->setEndValue(6);
        hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        QPropertyAnimation* opacityAnimation = new QPropertyAnimation(d, "pHoverOpacity");
        opacityAnimation->setDuration(130);
        opacityAnimation->setStartValue(d->_pHoverOpacity);
        opacityAnimation->setEndValue(1);
        opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        break;
    }
    case QEvent::Leave:
    {
        d->_floatTimer->stop();
        QPropertyAnimation* hoverAnimation = new QPropertyAnimation(d, "pHoverYOffset");
        connect(hoverAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            update();
        });
        hoverAnimation->setDuration(130);
        hoverAnimation->setStartValue(d->_pHoverYOffset);
        hoverAnimation->setEndValue(0);
        hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        QPropertyAnimation* opacityAnimation = new QPropertyAnimation(d, "pHoverOpacity");
        opacityAnimation->setDuration(130);
        opacityAnimation->setStartValue(d->_pHoverOpacity);
        opacityAnimation->setEndValue(0);
        opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        break;
    }
    case QEvent::MouseButtonRelease:
    {
        Q_EMIT popularCardClicked();
        break;
    }
    default:
    {
        break;
    }
    }
    return QWidget::event(event);
}

void ElaPopularCard::paintEvent(QPaintEvent* event)
{
    Q_D(ElaPopularCard);
    if (d->_isFloating)
    {
        return;
    }
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    if (underMouse())
    {
        // 阴影绘制
        painter.setOpacity(d->_pHoverOpacity);
        QRect shadowRect = rect().adjusted(0, -d->_pHoverYOffset, 0, -d->_pHoverYOffset);
        eTheme->drawEffectShadow(&painter, shadowRect, d->_shadowBorderWidth, d->_pBorderRadius);
    }
    QRectF foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth - d->_pHoverYOffset + 1, width() - 2 * d->_shadowBorderWidth, height() - 2 * d->_shadowBorderWidth);
    //背景绘制
    painter.setOpacity(1);
    painter.setPen(underMouse() ? ElaThemeColor(d->_themeMode, PopupBorderHover) : ElaThemeColor(d->_themeMode, BasicBorder));
    painter.setBrush(ElaThemeColor(d->_themeMode, BasicBaseAlpha));
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    //图片绘制
    painter.save();
    QRectF pixRect(foregroundRect.x() + foregroundRect.height() * 0.15, foregroundRect.y() + foregroundRect.height() * 0.15, foregroundRect.height() * 0.7, foregroundRect.height() * 0.7);
    QPainterPath pixPath;
    pixPath.addRoundedRect(pixRect, 4, 4);
    painter.setClipPath(pixPath);
    painter.drawPixmap(pixRect, d->_pCardPixmap, d->_pCardPixmap.rect());
    painter.restore();

    //文字绘制
    //计算按钮最终矩形
    int buttonTargetWidth = d->_floater->_overButton->fontMetrics().horizontalAdvance(d->_floater->_overButton->text()) + 40;
    if (buttonTargetWidth > width() * 0.25)
    {
        buttonTargetWidth = width() * 0.25;
    }
    //处理ElaPushButton的阴影偏移
    d->_buttonTargetRect = QRect(QPoint(width() + 2 * d->_floater->_floatGeometryOffset - d->_shadowBorderWidth + 3 - foregroundRect.height() * 0.15 - buttonTargetWidth, foregroundRect.height() * 0.15 - 3), QSize(buttonTargetWidth, 36));

    // Title
    painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
    QFont font = painter.font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(15);
    painter.setFont(font);
    int titleHeight = painter.fontMetrics().height();
    QRectF titleRect(pixRect.right() + d->_textHSpacing, pixRect.y(), d->_floater->_floatGeometryOffset * 2 + foregroundRect.width() - pixRect.width() - d->_textHSpacing * 2 - foregroundRect.height() * 0.15 - buttonTargetWidth, titleHeight);
    QString titleText = painter.fontMetrics().elidedText(d->_pTitle, Qt::ElideRight, titleRect.width());
    painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, titleText);

    // SubTitle
    font.setWeight(QFont::DemiBold);
    font.setPixelSize(13);
    painter.setFont(font);
    int subTitleHeight = painter.fontMetrics().height();
    QRectF subTitleRect(pixRect.right() + d->_textHSpacing, titleRect.bottom() + d->_textVSpacing, d->_floater->_floatGeometryOffset * 2 + foregroundRect.width() - pixRect.width() - d->_textHSpacing * 2 - foregroundRect.height() * 0.15 - buttonTargetWidth,
                        subTitleHeight);
    QString subTitleText = painter.fontMetrics().elidedText(d->_pSubTitle, Qt::ElideRight, subTitleRect.width());
    painter.drawText(subTitleRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, subTitleText);

    // InteractiveTips
    int tipWidth = painter.fontMetrics().horizontalAdvance(d->_pInteractiveTips);
    int tipHeight = painter.fontMetrics().height();
    if (!d->_pInteractiveTips.isEmpty())
    {
        font.setWeight(QFont::DemiBold);
        font.setPixelSize(12);
        painter.setFont(font);
        //覆盖背景绘制
        QRectF tipRect(foregroundRect.right() - d->_textHSpacing - tipWidth, foregroundRect.bottom() - d->_textHSpacing - tipHeight, foregroundRect.width() / 2 - d->_textHSpacing, tipHeight);
        painter.setPen(Qt::NoPen);
        painter.setBrush(ElaThemeColor(d->_themeMode, BasicBaseDeep));
        QRectF baseRect = tipRect;
        baseRect.setRight(tipRect.x() + tipWidth);
        baseRect.adjust(-7, -3, 4, 3);
        d->_interactiveTipsBaseRect = baseRect;
        painter.drawRoundedRect(baseRect, 6, 6);
        //文字绘制
        painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
        painter.drawText(tipRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, d->_pInteractiveTips);
    }
    else
    {
        QRectF tipRect(foregroundRect.right() - d->_textHSpacing - 50, foregroundRect.bottom() - d->_textHSpacing - tipHeight, foregroundRect.width() / 2 - d->_textHSpacing, tipHeight);
        tipRect.setRight(tipRect.x() + tipWidth);
        tipRect.adjust(-7, -3, 4, 3);
        d->_interactiveTipsBaseRect = tipRect;
    }
    painter.restore();
}
