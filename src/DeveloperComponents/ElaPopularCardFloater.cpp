#include "ElaPopularCardFloater.h"

#include <QEvent>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "ElaPopularCard.h"
#include "ElaPopularCardPrivate.h"
#include "ElaPushButton.h"
#include "ElaTheme.h"
ElaPopularCardFloater::ElaPopularCardFloater(ElaPopularCard* card, ElaPopularCardPrivate* cardPrivate, QWidget* parent)
    : QWidget{parent}
{
    _card = card;
    _cardPrivate = cardPrivate;
    _pHoverOpacity = 0;
    _pHoverYOffset = 0;
    setObjectName("ElaPopularCardFloater");
    setStyleSheet("#ElaPopularCardFloater{background-color:transparent}");
    setMouseTracking(true);

    _overButton = new ElaPushButton("获取", this);
    _opacityEffect = new QGraphicsOpacityEffect(_overButton);
    _opacityEffect->setOpacity(1);
    _overButton->setGraphicsEffect(_opacityEffect);
    _overButton->move(0, 0);
    _overButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, ContentDialogRightButtonBase));
    _overButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, ContentDialogRightButtonHover));
    _overButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, ContentDialogRightButtonPress));
    _overButton->setLightTextColor(ElaThemeColor(ElaThemeType::Light, ContentDialogRightButtonText));
    _overButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, ContentDialogRightButtonBase));
    _overButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, ContentDialogRightButtonHover));
    _overButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, ContentDialogRightButtonPress));
    _overButton->setDarkTextColor(ElaThemeColor(ElaThemeType::Dark, ContentDialogRightButtonText));
    _overButton->setMinimumSize(0, 0);
    _overButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    connect(_overButton, &ElaPushButton::clicked, _card, &ElaPopularCard::popularCardButtonClicked);

    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
    setVisible(false);
}

ElaPopularCardFloater::~ElaPopularCardFloater()
{
}

void ElaPopularCardFloater::showFloater()
{
    _isHideAnimationFinished = true;
    _pHoverYOffset = 6;
    _pHoverOpacity = 1;
    _opacityEffect->setOpacity(1);

    QPropertyAnimation* geometryAnimation = new QPropertyAnimation(this, "geometry");
    connect(geometryAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
        update();
    });
    geometryAnimation->setEasingCurve(QEasingCurve::OutQuad);
    geometryAnimation->setDuration(300);
    QRect cardGeometry = QRect(_card->mapTo(_cardPrivate->_pCardFloatArea, QPoint(0, 0)), _card->size());
    QRect endGeometry = _calculateTargetGeometry(cardGeometry);
    QRect startGeometry = QRect(endGeometry.x() + _floatGeometryOffset, endGeometry.y() + _floatGeometryOffset, _card->width(), _card->height());
    geometryAnimation->setStartValue(startGeometry);
    geometryAnimation->setEndValue(endGeometry);
    geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    //Button动画
    QPropertyAnimation* buttonAnimation = new QPropertyAnimation(_overButton, "geometry");
    buttonAnimation->setEasingCurve(QEasingCurve::OutQuad);
    buttonAnimation->setDuration(300);
    buttonAnimation->setStartValue(_cardPrivate->_interactiveTipsBaseRect);
    buttonAnimation->setEndValue(_cardPrivate->_buttonTargetRect);
    buttonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    setVisible(true);
}

void ElaPopularCardFloater::hideFloater()
{
    if (!_isHideAnimationFinished)
    {
        return;
    }
    _isHideAnimationFinished = false;
    QPropertyAnimation* geometryAnimation = new QPropertyAnimation(this, "geometry");
    connect(geometryAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
        QRect endGeometry = QRect(_card->mapTo(_cardPrivate->_pCardFloatArea, QPoint(0, 0)), _card->size());
        geometryAnimation->setEndValue(endGeometry);
    });
    connect(geometryAnimation, &QPropertyAnimation::finished, this, [=]() {
        _cardPrivate->_isFloating = false;
        setVisible(false);
        _card->update();
    });
    geometryAnimation->setEasingCurve(QEasingCurve::InOutSine);
    geometryAnimation->setDuration(300);
    QRect endGeometry = QRect(_card->mapTo(_cardPrivate->_pCardFloatArea, QPoint(0, 0)), _card->size());
    geometryAnimation->setStartValue(geometry());
    geometryAnimation->setEndValue(endGeometry);
    geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "pHoverOpacity");
    opacityAnimation->setDuration(300);
    opacityAnimation->setStartValue(_pHoverOpacity);
    opacityAnimation->setEndValue(0);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    //内容调整动画
    QPropertyAnimation* hoverAnimation = new QPropertyAnimation(this, "pHoverYOffset");
    connect(hoverAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
        update();
    });
    hoverAnimation->setDuration(300);
    hoverAnimation->setStartValue(_pHoverYOffset);
    hoverAnimation->setEndValue(0);
    hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    //Button动画
    QPropertyAnimation* buttonAnimation = new QPropertyAnimation(_overButton, "geometry");
    buttonAnimation->setEasingCurve(QEasingCurve::InOutSine);
    buttonAnimation->setDuration(300);
    buttonAnimation->setStartValue(_overButton->geometry());
    QRectF endRect = _cardPrivate->_interactiveTipsBaseRect;
    endRect.adjust(0, 6, 0, 6);
    buttonAnimation->setEndValue(endRect);
    buttonAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    QPropertyAnimation* buttonOpacityAnimation = new QPropertyAnimation(_overButton->graphicsEffect(), "opacity");
    buttonOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    buttonOpacityAnimation->setDuration(350);
    buttonOpacityAnimation->setStartValue(1);
    buttonOpacityAnimation->setEndValue(0);
    buttonOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

bool ElaPopularCardFloater::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Leave:
    {
        hideFloater();
        break;
    }
    case QEvent::MouseButtonRelease:
    {
        Q_EMIT _card->popularCardClicked();
        break;
    }
    default:
    {
        break;
    }
    }
    return QWidget::event(event);
}

void ElaPopularCardFloater::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 阴影绘制
    painter.setOpacity(_pHoverOpacity);
    QRect shadowRect = rect();
    shadowRect.adjust(0, 0, 0, -_pHoverYOffset);
    eTheme->drawEffectShadow(&painter, shadowRect, _cardPrivate->_shadowBorderWidth, _cardPrivate->_pBorderRadius);
    QRectF foregroundRect(_cardPrivate->_shadowBorderWidth, _cardPrivate->_shadowBorderWidth - _pHoverYOffset + 1, width() - 2 * _cardPrivate->_shadowBorderWidth, height() - 2 * _cardPrivate->_shadowBorderWidth);
    QRectF cardForegroundRect(_cardPrivate->_shadowBorderWidth, _cardPrivate->_shadowBorderWidth - _cardPrivate->_pHoverYOffset + 1, _card->width() - 2 * _cardPrivate->_shadowBorderWidth, _card->height() - 2 * _cardPrivate->_shadowBorderWidth);

    //背景绘制
    painter.setOpacity(1);
    painter.setPen(ElaThemeColor(_themeMode, PopularCardFloaterBorder));
    painter.setBrush(ElaThemeColor(_themeMode, PopularCardBase));
    painter.drawRoundedRect(foregroundRect, _cardPrivate->_pBorderRadius, _cardPrivate->_pBorderRadius);
    painter.setClipRect(foregroundRect);
    //图片绘制
    painter.save();
    QRectF pixRect(foregroundRect.x() + cardForegroundRect.height() * 0.15, foregroundRect.y() + cardForegroundRect.height() * 0.15, cardForegroundRect.height() * 0.7, cardForegroundRect.height() * 0.7);

    QPainterPath pixPath;
    pixPath.addRoundedRect(pixRect, 4, 4);
    painter.setClipPath(pixPath);
    painter.drawPixmap(pixRect, _cardPrivate->_pCardPixmap, _cardPrivate->_pCardPixmap.rect());
    painter.restore();

    //文字绘制
    //Title
    painter.setPen(ElaThemeColor(_themeMode, WindowText));
    QFont font = painter.font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(15);
    painter.setFont(font);
    int titleHeight = painter.fontMetrics().height();
    QRectF titleRect(pixRect.right() + _cardPrivate->_textHSpacing, pixRect.y(), _floatGeometryOffset * 2 + cardForegroundRect.width() - pixRect.width() - _cardPrivate->_textHSpacing * 2 - cardForegroundRect.height() * 0.15 - _cardPrivate->_buttonTargetRect.width(), titleHeight);
    QString titleText = painter.fontMetrics().elidedText(_cardPrivate->_pTitle, Qt::ElideRight, titleRect.width());
    painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, titleText);

    //SubTitle
    font.setWeight(QFont::DemiBold);
    font.setPixelSize(13);
    painter.setFont(font);
    int subTitleHeight = painter.fontMetrics().height();
    QRectF subTitleRect(pixRect.right() + _cardPrivate->_textHSpacing, titleRect.bottom() + _cardPrivate->_textVSpacing, _floatGeometryOffset * 2 + cardForegroundRect.width() - pixRect.width() - _cardPrivate->_textHSpacing * 2 - cardForegroundRect.height() * 0.15 - _cardPrivate->_buttonTargetRect.width(), subTitleHeight);
    QString subTitleText = painter.fontMetrics().elidedText(_cardPrivate->_pSubTitle, Qt::ElideRight, subTitleRect.width());
    painter.drawText(subTitleRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, subTitleText);

    //DetailedText
    painter.setPen(ElaThemeColor(_themeMode, PopularCardDetailedText));
    int detailedTextHeight = painter.fontMetrics().height() * 2 + 2;
    QRectF detailedTextRect(pixRect.x(), pixRect.bottom() + cardForegroundRect.height() * 0.15, cardForegroundRect.width() + 2 * _floatGeometryOffset - _cardPrivate->_textHSpacing - cardForegroundRect.height() * 0.15, detailedTextHeight);
    QString detailedText = painter.fontMetrics().elidedText(_cardPrivate->_pDetailedText, Qt::ElideRight, detailedTextRect.width() * 1.9);
    painter.drawText(detailedTextRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, detailedText);

    //分割线绘制
    painter.setPen(ElaThemeColor(_themeMode, PopularCardFloaterBaseLine));
    painter.drawLine(foregroundRect.x(), detailedTextRect.bottom() + 5, foregroundRect.right(), detailedTextRect.bottom() + 5);

    //CardFloatPixmap
    painter.drawPixmap(QRect(pixRect.x(), detailedTextRect.bottom() + 15, cardForegroundRect.bottom() + _floatGeometryOffset + 90 - detailedTextRect.bottom() - 15 - _cardPrivate->_shadowBorderWidth - 10, cardForegroundRect.bottom() + _floatGeometryOffset + 90 - detailedTextRect.bottom() - 15 - _cardPrivate->_shadowBorderWidth - 10), _cardPrivate->_pCardFloatPixmap);
    painter.restore();
}

QRect ElaPopularCardFloater::_calculateTargetGeometry(QRect cardGeometry)
{
    QRect targetGeometry = cardGeometry;
    targetGeometry.adjust(-_floatGeometryOffset, -_floatGeometryOffset, _floatGeometryOffset, 90);
    QRect windowRect = _cardPrivate->_pCardFloatArea->rect();
    if (targetGeometry.bottom() > windowRect.bottom())
    {
        int offset = targetGeometry.bottom() - windowRect.bottom();
        targetGeometry.adjust(0, -offset, 0, -offset);
    }
    if (targetGeometry.top() < windowRect.top())
    {
        int offset = targetGeometry.top() - windowRect.top();
        targetGeometry.adjust(0, -offset, 0, -offset);
    }
    return targetGeometry;
}
