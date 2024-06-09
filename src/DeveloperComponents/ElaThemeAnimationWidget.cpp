#include "ElaThemeAnimationWidget.h"

#include <Windows.h>

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
ElaThemeAnimationWidget::ElaThemeAnimationWidget(QWidget* parent)
    : QWidget{parent}
{
    _pBorderRadius = 6.5;
    _pEndRadius = 0.01;
    _pIsAnimationFinished = true;
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground, true);
    _pIsMaximum = false;
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
        _pIsAnimationFinished = true;
        setVisible(false); });
    connect(themeChangeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { update(); });
    themeChangeAnimation->setStartValue(0);
    themeChangeAnimation->setEndValue(_pEndRadius);
    themeChangeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    _pIsAnimationFinished = false;
    setVisible(true);
    HWND hwnd = reinterpret_cast<HWND>(winId());
    ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void ElaThemeAnimationWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    QPainterPath clipPath;
    if (_pIsMaximum)
    {
        clipPath.addRect(QRect(rect().x(), rect().y(), rect().width(), rect().height()));
    }
    else
    {
        clipPath.addRoundedRect(QRectF(rect().x() + 0.75, rect().y() + 1, rect().width() - 1.5, rect().height() - 1), _pBorderRadius, _pBorderRadius);
    }

    painter.setClipPath(clipPath);
    painter.drawImage(rect(), _pWindowBackground);
    QPainterPath path;
    path.moveTo(_pCenter.x(), _pCenter.y());
    path.addEllipse(QPointF(_pCenter.x(), _pCenter.y()), _pRadius, _pRadius);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillPath(path, Qt::black);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.fillRect(rect(), QColor(0, 0, 0, 1));
    painter.restore();
}
