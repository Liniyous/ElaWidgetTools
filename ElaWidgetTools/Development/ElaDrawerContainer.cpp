#include "ElaDrawerContainer.h"

#include "ElaColorAnimation.h"
#include "ElaControlStyle.h"
#include "ElaTheme.h"

#include <QEasingCurve>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QVariantAnimation>

ElaDrawerContainer::ElaDrawerContainer(QWidget* parent)
    : QWidget(parent)
{
    _pBorderRadius = 6;
    setObjectName("ElaDrawerContainer");
    setStyleSheet("#ElaDrawerContainer{background-color:transparent;}");

    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(1, 0, 1, 1);
    _mainLayout->setSpacing(0);

    _containerWidget = new QWidget(this);
    _containerWidget->setObjectName("ElaDrawerContainerWidget");
    _containerWidget->setStyleSheet("#ElaDrawerContainerWidget{background-color:transparent;}");
    _containerWidget->setVisible(false);

    _containerLayout = new QVBoxLayout(_containerWidget);
    _containerLayout->setContentsMargins(0, 0, 0, 0);
    _containerLayout->setSpacing(0);

    _mainLayout->addWidget(_containerWidget);
    setFixedHeight(0);
    hide();

    _heightAnimation = new QVariantAnimation(this);
    connect(_heightAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
        const int currentHeight = value.toInt();
        setFixedHeight(currentHeight);
        _containerWidget->setGeometry(1, currentHeight - 1 - _contentHeight, qMax(0, width() - 2), _contentHeight);
        update();
    });
    connect(_heightAnimation, &QVariantAnimation::finished, this, [this]() {
        _applyExpandStateImmediate(_isExpanded);
        if (!_isExpanded)
        {
            Q_EMIT containerCollapsed();
        }
    });

    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
}

ElaDrawerContainer::~ElaDrawerContainer()
{
}

void ElaDrawerContainer::addWidget(QWidget* widget)
{
    if (!widget || _drawerWidgetList.contains(widget))
    {
        return;
    }
    _containerLayout->addWidget(widget);
    _drawerWidgetList.append(widget);
    if (_isExpanded && !_isAnimating)
    {
        _applyExpandStateImmediate(true);
    }
}

void ElaDrawerContainer::removeWidget(QWidget* widget)
{
    if (!widget)
    {
        return;
    }
    _containerLayout->removeWidget(widget);
    _drawerWidgetList.removeOne(widget);
    if (_drawerWidgetList.isEmpty())
    {
        _applyExpandStateImmediate(false);
        Q_EMIT containerCollapsed();
    }
    else if (_isExpanded && !_isAnimating)
    {
        _applyExpandStateImmediate(true);
    }
}

bool ElaDrawerContainer::getHasWidgets() const
{
    return !_drawerWidgetList.isEmpty();
}

void ElaDrawerContainer::doDrawerAnimation(bool isExpand)
{
    _isExpanded = isExpand;
    if (_drawerWidgetList.isEmpty())
    {
        _applyExpandStateImmediate(false);
        Q_EMIT containerCollapsed();
        return;
    }
    if (!parentWidget() || !parentWidget()->isVisible() || !ElaColorAnimation::getIsAnimationEnabled(this))
    {
        _applyExpandStateImmediate(isExpand);
        if (!isExpand)
        {
            Q_EMIT containerCollapsed();
        }
        return;
    }

    const int startHeight = isVisible() ? height() : 0;
    _heightAnimation->stop();
    _mainLayout->setEnabled(false);
    _containerWidget->setVisible(true);
    _containerWidget->resize(qMax(0, width() - 2), _containerWidget->sizeHint().height());
    _contentHeight = _getContentHeight();
    const int targetHeight = _contentHeight > 0 ? _contentHeight + 1 : 0;
    const int endHeight = isExpand ? targetHeight : 0;

    if (targetHeight <= 0 || startHeight == endHeight)
    {
        _applyExpandStateImmediate(isExpand);
        if (!isExpand)
        {
            Q_EMIT containerCollapsed();
        }
        return;
    }

    _isAnimating = true;
    setFixedHeight(startHeight);
    _containerWidget->setGeometry(1, startHeight - 1 - _contentHeight, qMax(0, width() - 2), _contentHeight);
    show();

    QEasingCurve easing(QEasingCurve::BezierSpline);
    easing.addCubicBezierSegment(QPointF(0.0, 0.0), QPointF(0.0, 1.0), QPointF(1.0, 1.0));
    _heightAnimation->setEasingCurve(easing);
    _heightAnimation->setDuration(isExpand ? 250 : 200);
    _heightAnimation->setStartValue(startHeight);
    _heightAnimation->setEndValue(endHeight);
    _heightAnimation->start();
}

void ElaDrawerContainer::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (_isAnimating && _containerWidget)
    {
        _containerWidget->setGeometry(1, height() - 1 - _contentHeight, qMax(0, width() - 2), _contentHeight);
    }
}

void ElaDrawerContainer::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    if (height() <= 0 || (!_isExpanded && !_isAnimating))
    {
        return;
    }
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);

    const QRectF bodyRect(0.5, 0.0, qMax(0.0, width() - 1.0), qMax(0.5, height() - 0.5));
    const qreal radius = qBound(0.0, static_cast<qreal>(_pBorderRadius), qMin(bodyRect.width() / 2.0, bodyRect.height()));

    QPainterPath fillPath;
    fillPath.moveTo(bodyRect.left(), 0.0);
    fillPath.lineTo(bodyRect.left(), bodyRect.bottom() - radius);
    if (radius > 0.0)
    {
        fillPath.arcTo(QRectF(bodyRect.left(), bodyRect.bottom() - 2.0 * radius, 2.0 * radius, 2.0 * radius), 180.0, 90.0);
        fillPath.lineTo(bodyRect.right() - radius, bodyRect.bottom());
        fillPath.arcTo(QRectF(bodyRect.right() - 2.0 * radius, bodyRect.bottom() - 2.0 * radius, 2.0 * radius, 2.0 * radius), -90.0, 90.0);
    }
    else
    {
        fillPath.lineTo(bodyRect.left(), bodyRect.bottom());
        fillPath.lineTo(bodyRect.right(), bodyRect.bottom());
    }
    fillPath.lineTo(bodyRect.right(), 0.0);
    fillPath.closeSubpath();

    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(_themeMode, BasicBaseAlpha), "DrawerContainerBackground"));
    painter.drawPath(fillPath);

    // 子项分隔线随内部面板滑动同步位移并限制在圆角区域内
    painter.save();
    painter.setClipPath(fillPath);
    painter.setPen(QPen(ElaThemeColor(_themeMode, BasicBorder), 1.0));
    for (int i = 0; i < _drawerWidgetList.count() - 1; ++i)
    {
        const QWidget* drawerWidget = _drawerWidgetList.at(i);
        if (!drawerWidget || !drawerWidget->isVisible())
        {
            continue;
        }
        const qreal sepY = _containerWidget->y() + drawerWidget->geometry().bottom() + 0.5;
        if (sepY > 0.5 && sepY < bodyRect.bottom() - 0.5)
        {
            painter.drawLine(QLineF(1.0, sepY, width() - 1.0, sepY));
        }
    }
    painter.restore();

    QPainterPath borderPath;
    borderPath.moveTo(bodyRect.left(), 0.0);
    borderPath.lineTo(bodyRect.left(), bodyRect.bottom() - radius);
    if (radius > 0.0)
    {
        borderPath.arcTo(QRectF(bodyRect.left(), bodyRect.bottom() - 2.0 * radius, 2.0 * radius, 2.0 * radius), 180.0, 90.0);
        borderPath.lineTo(bodyRect.right() - radius, bodyRect.bottom());
        borderPath.arcTo(QRectF(bodyRect.right() - 2.0 * radius, bodyRect.bottom() - 2.0 * radius, 2.0 * radius, 2.0 * radius), -90.0, 90.0);
    }
    else
    {
        borderPath.lineTo(bodyRect.left(), bodyRect.bottom());
        borderPath.lineTo(bodyRect.right(), bodyRect.bottom());
    }
    borderPath.lineTo(bodyRect.right(), 0.0);
    const bool isBottomElevated = _themeMode == ElaThemeType::Light;
    painter.setPen(QPen(ElaControlStyle::getControlBorder(bodyRect, _themeMode, isBottomElevated), 1.0));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(borderPath);

    painter.restore();
}

int ElaDrawerContainer::_getContentHeight() const
{
    if (_drawerWidgetList.isEmpty())
    {
        return 0;
    }
    _containerLayout->activate();
    return qMax(0, _containerLayout->totalSizeHint().height());
}

void ElaDrawerContainer::_applyExpandStateImmediate(bool isExpand)
{
    _heightAnimation->stop();
    _isAnimating = false;
    _isExpanded = isExpand && !_drawerWidgetList.isEmpty();
    if (_isExpanded)
    {
        _containerWidget->setVisible(true);
        _mainLayout->setEnabled(true);
        setMinimumHeight(0);
        setMaximumHeight(QWIDGETSIZE_MAX);
        _mainLayout->activate();
        show();
        updateGeometry();
        update();
    }
    else
    {
        _containerWidget->setVisible(false);
        _mainLayout->setEnabled(true);
        setFixedHeight(0);
        hide();
        updateGeometry();
    }
}
