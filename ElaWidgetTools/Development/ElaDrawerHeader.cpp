#include "ElaDrawerHeader.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
#include "ElaControlStyle.h"
#include "ElaTheme.h"

#include <QEasingCurve>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

ElaDrawerHeader::ElaDrawerHeader(QWidget* parent)
    : QWidget(parent)
{
    _pBorderRadius = 6;
    _pIsExpand = false;
    _pExpandIconRotate = 0;
    setFixedHeight(75);
    setMouseTracking(true);
    setObjectName("ElaDrawerHeader");
    setStyleSheet("#ElaDrawerHeader{background-color:transparent;}");

    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 30, 0);

    _rotateAnimation = new QPropertyAnimation(this, "pExpandIconRotate", this);
    connect(_rotateAnimation, &QPropertyAnimation::valueChanged, this, [this]() {
        update();
    });

    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
}

ElaDrawerHeader::~ElaDrawerHeader()
{
}

void ElaDrawerHeader::setHeaderWidget(QWidget* widget)
{
    if (!widget)
    {
        return;
    }
    if (_headerWidget)
    {
        _mainLayout->removeWidget(_headerWidget);
        _headerWidget->deleteLater();
    }
    _mainLayout->addWidget(widget);
    _headerWidget = widget;
}

void ElaDrawerHeader::setIsBottomAttached(bool isAttached)
{
    if (_isBottomAttached == isAttached)
    {
        return;
    }
    _isBottomAttached = isAttached;
    update();
}

bool ElaDrawerHeader::getIsBottomAttached() const
{
    return _isBottomAttached;
}

void ElaDrawerHeader::doExpandOrCollapseAnimation()
{
    _rotateAnimation->stop();
    const qreal targetAngle = _pIsExpand ? -180.0 : 0.0;
    if (!isVisible() || !ElaColorAnimation::getIsAnimationEnabled(this))
    {
        _pExpandIconRotate = targetAngle;
        update();
        return;
    }
    QEasingCurve easing(QEasingCurve::BezierSpline);
    easing.addCubicBezierSegment(QPointF(0.0, 0.0), QPointF(0.0, 1.0), QPointF(1.0, 1.0));
    _rotateAnimation->setEasingCurve(easing);
    _rotateAnimation->setDuration(_pIsExpand ? 250 : 200);
    _rotateAnimation->setStartValue(_pExpandIconRotate);
    _rotateAnimation->setEndValue(targetAngle);
    _rotateAnimation->start();
}

bool ElaDrawerHeader::event(QEvent* event)
{
    if (event->type() == QEvent::Enter || event->type() == QEvent::Leave)
    {
        update();
    }
    return QWidget::event(event);
}

void ElaDrawerHeader::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && _getIsHeaderClickable(childAt(event->pos())))
    {
        _isPressed = true;
        update();
        event->accept();
        return;
    }
    QWidget::mousePressEvent(event);
}

void ElaDrawerHeader::mouseReleaseEvent(QMouseEvent* event)
{
    const bool wasPressed = _isPressed;
    if (_isPressed)
    {
        _isPressed = false;
        update();
    }
    if (wasPressed && event->button() == Qt::LeftButton)
    {
        if (rect().contains(event->pos()) && _getIsHeaderClickable(childAt(event->pos())))
        {
            _pIsExpand = !_pIsExpand;
            doExpandOrCollapseAnimation();
            Q_EMIT drawerHeaderClicked(_pIsExpand);
        }
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void ElaDrawerHeader::mouseMoveEvent(QMouseEvent* event)
{
    QWidget::mouseMoveEvent(event);
}

void ElaDrawerHeader::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QStyle::State state = QStyle::State_None;
    if (isEnabled())
    {
        state |= QStyle::State_Enabled;
    }
    if (_isPressed)
    {
        state |= QStyle::State_Sunken;
    }
    else if (underMouse())
    {
        state |= QStyle::State_MouseOver;
    }
    const QColor bgColor = ElaColorAnimation::getColor(this, ElaControlStyle::getControlBackgroundColor(_themeMode, state), "DrawerHeaderBackground");
    const qreal radius = qBound(0.0, static_cast<qreal>(_pBorderRadius), qMin(width(), height()) / 2.0 - 0.5);

    if (!_isBottomAttached)
    {
        const QRectF headerRect(0.5, 0.5, qMax(0.0, width() - 1.0), qMax(0.0, height() - 1.0));
        painter.setPen(QPen(ElaControlStyle::getControlBorder(headerRect, _themeMode, !_isPressed), 1.0));
        painter.setBrush(bgColor);
        painter.drawRoundedRect(headerRect, radius, radius);
    }
    else
    {
        // 展开期间保持底部直角与下方容器拼接 统一作为单张卡片表面过渡
        const QRectF headerRect(0.5, 0.5, qMax(0.0, width() - 1.0), qMax(0.0, height() - 0.5));
        QPainterPath fillPath;
        fillPath.moveTo(headerRect.left(), height());
        fillPath.lineTo(headerRect.left(), headerRect.top() + radius);
        if (radius > 0.0)
        {
            fillPath.arcTo(QRectF(headerRect.left(), headerRect.top(), 2.0 * radius, 2.0 * radius), 180.0, -90.0);
            fillPath.lineTo(headerRect.right() - radius, headerRect.top());
            fillPath.arcTo(QRectF(headerRect.right() - 2.0 * radius, headerRect.top(), 2.0 * radius, 2.0 * radius), 90.0, -90.0);
        }
        else
        {
            fillPath.lineTo(headerRect.left(), headerRect.top());
            fillPath.lineTo(headerRect.right(), headerRect.top());
        }
        fillPath.lineTo(headerRect.right(), height());
        fillPath.closeSubpath();

        painter.setPen(Qt::NoPen);
        painter.setBrush(bgColor);
        painter.drawPath(fillPath);

        QPainterPath borderPath;
        borderPath.moveTo(headerRect.left(), height());
        borderPath.lineTo(headerRect.left(), headerRect.top() + radius);
        if (radius > 0.0)
        {
            borderPath.arcTo(QRectF(headerRect.left(), headerRect.top(), 2.0 * radius, 2.0 * radius), 180.0, -90.0);
            borderPath.lineTo(headerRect.right() - radius, headerRect.top());
            borderPath.arcTo(QRectF(headerRect.right() - 2.0 * radius, headerRect.top(), 2.0 * radius, 2.0 * radius), 90.0, -90.0);
        }
        else
        {
            borderPath.lineTo(headerRect.left(), headerRect.top());
            borderPath.lineTo(headerRect.right(), headerRect.top());
        }
        borderPath.lineTo(headerRect.right(), height());
        const bool isTopSheen = _themeMode == ElaThemeType::Dark && !_isPressed;
        painter.setPen(QPen(ElaControlStyle::getControlBorder(headerRect, _themeMode, isTopSheen), 1.0));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(borderPath);

        painter.setPen(QPen(ElaThemeColor(_themeMode, BasicBorder), 1.0));
        painter.drawLine(QLineF(1.0, height() - 0.5, width() - 1.0, height() - 0.5));
    }

    // 围绕图标几何中心旋转展开箭头
    QFont iconFont("ElaAwesome");
    iconFont.setPixelSize(eApp->getFontPixelSize() + 3);
    painter.setFont(iconFont);
    painter.setPen(isEnabled() ? ElaThemeColor(_themeMode, BasicText) : ElaThemeColor(_themeMode, BasicTextDisable));
    const QPointF iconCenter(width() - 24.0, height() / 2.0 + (_isPressed ? 1.0 : 0.0));
    painter.translate(iconCenter);
    painter.rotate(_pExpandIconRotate);
    painter.drawText(QRectF(-12.0, -12.0, 24.0, 24.0), Qt::AlignCenter, QChar(ElaIconType::AngleDown));

    painter.restore();
}

bool ElaDrawerHeader::_getIsHeaderClickable(const QWidget* posWidget)
{
    if (!posWidget)
    {
        return true;
    }
    if (const auto* label = qobject_cast<const QLabel*>(posWidget))
    {
        return !label->textInteractionFlags().testFlag(Qt::TextEditable)
            && !label->textInteractionFlags().testFlag(Qt::TextSelectableByMouse);
    }
    return posWidget->objectName().isEmpty();
}
