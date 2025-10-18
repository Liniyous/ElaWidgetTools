#include "ElaDrawerHeader.h"
#include "ElaTheme.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
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

    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
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

void ElaDrawerHeader::doExpandOrCollapseAnimation()
{
    QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "pExpandIconRotate");
    connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    rotateAnimation->setDuration(300);
    rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
    rotateAnimation->setStartValue(_pExpandIconRotate);
    rotateAnimation->setEndValue(_pIsExpand ? -180 : 0);
    rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

bool ElaDrawerHeader::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Enter:
    case QEvent::Leave:
    {
        update();
        break;
    }
    default:
    {
        break;
    }
    }
    return QWidget::event(event);
}

void ElaDrawerHeader::mousePressEvent(QMouseEvent* event)
{
    QWidget* posWidget = childAt(event->pos());
    if (!posWidget || (posWidget && posWidget->objectName().isEmpty()))
    {
        _isPressed = true;
        update();
    }
    QWidget::mousePressEvent(event);
}

void ElaDrawerHeader::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget* posWidget = childAt(event->pos());
    if (!posWidget || (posWidget && posWidget->objectName().isEmpty()))
    {
        _isPressed = false;
        _pIsExpand = !_pIsExpand;
        //指示器动画
        doExpandOrCollapseAnimation();
        Q_EMIT drawerHeaderClicked(_pIsExpand);
    }
    QWidget::mouseReleaseEvent(event);
}

void ElaDrawerHeader::mouseMoveEvent(QMouseEvent* event)
{
    update();
    QWidget::mouseMoveEvent(event);
}

void ElaDrawerHeader::paintEvent(QPaintEvent* event)
{
    bool isUnderMouse = underMouse();
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(ElaThemeColor(_themeMode, BasicBorder));
    painter.setBrush(_isPressed ? ElaThemeColor(_themeMode, BasicSelectedAlpha) : isUnderMouse ? ElaThemeColor(_themeMode, BasicPressAlpha)
                                                                                               : ElaThemeColor(_themeMode, BasicBaseAlpha));
    QRect foregroundRect(1, 1, width() - 2, _pIsExpand ? height() + _pBorderRadius : height() - 2);
    painter.drawRoundedRect(foregroundRect, _pBorderRadius, _pBorderRadius);
    // 底边线绘制
    if (isUnderMouse)
    {
        painter.setPen(ElaThemeColor(_themeMode, BasicBorderDeep));
    }
    if (_pIsExpand)
    {
        painter.drawLine(0, height() - 1, width(), height() - 1);
    }
    else
    {
        painter.drawLine(_pBorderRadius, height() - 1, width() - _pBorderRadius, height() - 1);
        painter.drawArc(QRectF(1, height() - 1 - 2 * _pBorderRadius, 2 * _pBorderRadius, 2 * _pBorderRadius), 240 * 16, 30 * 16);
        painter.drawArc(QRectF(width() - 2 * _pBorderRadius, height() - 1 - 2 * _pBorderRadius, 2 * _pBorderRadius, 2 * _pBorderRadius), -90 * 16, 30 * 16);
    }

    // 展开图标绘制
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(17);
    painter.setFont(iconFont);
    painter.setPen(isEnabled() ? ElaThemeColor(_themeMode, BasicText) : ElaThemeColor(_themeMode, BasicTextDisable));
    QRectF expandIconRect(width() - 25, 0, 20, height());
    painter.translate(expandIconRect.x() + (qreal)expandIconRect.width() / 2 - 2, expandIconRect.y() + (qreal)expandIconRect.height() / 2);
    painter.rotate(_pExpandIconRotate);
    painter.translate(-expandIconRect.x() - (qreal)expandIconRect.width() / 2 + 2, -expandIconRect.y() - (qreal)expandIconRect.height() / 2);
    painter.drawText(expandIconRect, Qt::AlignVCenter, QChar((unsigned short)ElaIconType::AngleDown));

    painter.restore();
}
