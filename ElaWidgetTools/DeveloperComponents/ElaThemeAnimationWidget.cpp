#include "ElaThemeAnimationWidget.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
ElaThemeAnimationWidget::ElaThemeAnimationWidget(QWidget* parent)
    : QWidget{parent}
{
    _pEndRadius = 0.01;
}

ElaThemeAnimationWidget::~ElaThemeAnimationWidget()
{
}

void ElaThemeAnimationWidget::startAnimation(int msec)
{
    QPropertyAnimation* themeChangeAnimation = new QPropertyAnimation(this, "pRadius");
    themeChangeAnimation->setDuration(msec);
    themeChangeAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(themeChangeAnimation, &QPropertyAnimation::finished, this, [=]() {
        Q_EMIT animationFinished();
        this->deleteLater();
    });
    connect(themeChangeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    themeChangeAnimation->setStartValue(0);
    themeChangeAnimation->setEndValue(_pEndRadius);
    themeChangeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaThemeAnimationWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);

    // 合成图片
    QImage animationImage(_pOldWindowBackground.size(), QImage::Format_ARGB32);
    animationImage.fill(Qt::transparent);
    QPainter animationImagePainter(&animationImage);
    animationImagePainter.setRenderHints(QPainter::Antialiasing);
    animationImagePainter.drawImage(_pOldWindowBackground.rect(), _pOldWindowBackground);
    animationImagePainter.setCompositionMode(QPainter::CompositionMode::CompositionMode_SourceOut);
    qreal devicePixelRatioF = _pOldWindowBackground.devicePixelRatioF();
    QPainterPath clipPath;
    clipPath.moveTo(_pCenter.x() * devicePixelRatioF, _pCenter.y() * devicePixelRatioF);
    clipPath.addEllipse(QPointF(_pCenter.x() * devicePixelRatioF, _pCenter.y() * devicePixelRatioF), _pRadius * devicePixelRatioF, _pRadius * devicePixelRatioF);
    animationImagePainter.setClipPath(clipPath);
    animationImagePainter.drawImage(animationImage.rect(), animationImage);
    animationImagePainter.end();

    painter.drawImage(rect(), animationImage);
    painter.restore();
}
