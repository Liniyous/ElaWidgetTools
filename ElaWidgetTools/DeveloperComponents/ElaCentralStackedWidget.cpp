#include "ElaCentralStackedWidget.h"

#include "ElaTheme.h"
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTimer>
#include <cmath>
ElaCentralStackedWidget::ElaCentralStackedWidget(QWidget* parent)
    : QStackedWidget(parent)
{
    _pPopupAnimationYOffset = 0;
    _pScaleAnimationRatio = 1;
    _pScaleAnimationPixOpacity = 1;
    _pFlipAnimationRatio = 1;
    setObjectName("ElaCentralStackedWidget");
    setStyleSheet("#ElaCentralStackedWidget{background-color:transparent;}");
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, &ElaCentralStackedWidget::onThemeModeChanged);
}

ElaCentralStackedWidget::~ElaCentralStackedWidget()
{
}

void ElaCentralStackedWidget::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
}

void ElaCentralStackedWidget::setIsTransparent(bool isTransparent)
{
    this->_isTransparent = isTransparent;
    update();
}

bool ElaCentralStackedWidget::getIsTransparent() const
{
    return _isTransparent;
}

void ElaCentralStackedWidget::setIsHasRadius(bool isHasRadius)
{
    this->_isHasRadius = isHasRadius;
    update();
}

void ElaCentralStackedWidget::doWindowStackSwitch(ElaWindowType::StackSwitchMode stackSwitchMode, int nodeIndex, bool isRouteBack)
{
    _stackSwitchMode = stackSwitchMode;
    switch (stackSwitchMode)
    {
    case ElaWindowType::None:
    {
        this->setCurrentIndex(nodeIndex);
        break;
    }
    case ElaWindowType::Popup:
    {
        QTimer::singleShot(180, this, [=]() {
            QWidget* targetWidget = this->widget(nodeIndex);
            this->setCurrentIndex(nodeIndex);
            _getTargetStackPix();
            targetWidget->setVisible(false);
            QPropertyAnimation* popupAnimation = new QPropertyAnimation(this, "pPopupAnimationYOffset");
            connect(popupAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                update();
            });
            connect(popupAnimation, &QPropertyAnimation::finished, this, [=]() {
                _targetStackPix = QPixmap();
                targetWidget->setVisible(true);
            });
            popupAnimation->setEasingCurve(QEasingCurve::OutCubic);
            popupAnimation->setDuration(300);
            int targetWidgetY = targetWidget->y();
            popupAnimation->setEndValue(targetWidgetY);
            targetWidgetY += 80;
            popupAnimation->setStartValue(targetWidgetY);
            popupAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        });
        break;
    }
    case ElaWindowType::Scale:
    {
        QWidget* targetWidget = this->widget(nodeIndex);
        _getCurrentStackPix();
        this->setCurrentIndex(nodeIndex);
        _getTargetStackPix();
        targetWidget->setVisible(false);
        _isDrawNewPix = false;
        QPropertyAnimation* currentPixZoomAnimation = new QPropertyAnimation(this, "pScaleAnimationRatio");
        connect(currentPixZoomAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            update();
        });
        connect(currentPixZoomAnimation, &QPropertyAnimation::finished, this, [=]() {
            _isDrawNewPix = true;
            QPropertyAnimation* targetPixZoomAnimation = new QPropertyAnimation(this, "pScaleAnimationRatio");
            connect(targetPixZoomAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                update();
            });
            connect(targetPixZoomAnimation, &QPropertyAnimation::finished, this, [=]() {
                _targetStackPix = QPixmap();
                _currentStackPix = QPixmap();
                targetWidget->setVisible(true);
            });
            if (isRouteBack)
            {
                targetPixZoomAnimation->setStartValue(1.5);
                targetPixZoomAnimation->setEndValue(1);
            }
            else
            {
                // 放大
                targetPixZoomAnimation->setStartValue(0.85);
                targetPixZoomAnimation->setEndValue(1);
            }
            targetPixZoomAnimation->setDuration(300);
            targetPixZoomAnimation->setEasingCurve(QEasingCurve::OutCubic);
            targetPixZoomAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        });
        if (isRouteBack)
        {
            // 缩小
            currentPixZoomAnimation->setStartValue(1);
            currentPixZoomAnimation->setEndValue(0.85);
        }
        else
        {
            // 放大
            currentPixZoomAnimation->setStartValue(1);
            currentPixZoomAnimation->setEndValue(1.15);
        }
        currentPixZoomAnimation->setDuration(150);
        currentPixZoomAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* currentPixOpacityAnimation = new QPropertyAnimation(this, "pScaleAnimationPixOpacity");
        connect(currentPixZoomAnimation, &QPropertyAnimation::finished, this, [=]() {
            QPropertyAnimation* targetPixOpacityAnimation = new QPropertyAnimation(this, "pScaleAnimationPixOpacity");
            targetPixOpacityAnimation->setStartValue(0);
            targetPixOpacityAnimation->setEndValue(1);
            targetPixOpacityAnimation->setDuration(300);
            targetPixOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        });
        currentPixOpacityAnimation->setStartValue(1);
        currentPixOpacityAnimation->setEndValue(0);
        currentPixOpacityAnimation->setDuration(150);
        currentPixOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        break;
    }
    case ElaWindowType::Flip:
    {
        QWidget* targetWidget = this->widget(nodeIndex);
        _getCurrentStackPix();
        this->setCurrentIndex(nodeIndex);
        _getTargetStackPix();
        targetWidget->setVisible(false);
        QPropertyAnimation* flipAnimation = new QPropertyAnimation(this, "pFlipAnimationRatio");
        connect(flipAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            update();
        });
        connect(flipAnimation, &QPropertyAnimation::finished, this, [=]() {
            _targetStackPix = QPixmap();
            _currentStackPix = QPixmap();
            targetWidget->setVisible(true);
        });
        flipAnimation->setEasingCurve(QEasingCurve::InOutSine);
        flipAnimation->setDuration(650);
        flipAnimation->setStartValue(0);
        flipAnimation->setEndValue(isRouteBack ? -180 : 180);
        flipAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        break;
    }
    }
}

void ElaCentralStackedWidget::paintEvent(QPaintEvent* event)
{
    QRect targetRect = this->rect();
    targetRect.adjust(1, 1, 10, 10);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    if (!_isTransparent)
    {
        painter.setPen(QPen(ElaThemeColor(_themeMode, BasicBaseLine), 1.5));
        painter.setBrush(ElaThemeColor(_themeMode, WindowCentralStackBase));
        if (_isHasRadius)
        {
            painter.drawRoundedRect(targetRect, 10, 10);
        }
        else
        {
            painter.drawRect(targetRect);
        }
    }
    // 切换动画
    if (!_targetStackPix.isNull())
    {
        QPainterPath clipPath;
        clipPath.addRoundedRect(targetRect, 10, 10);
        painter.setClipPath(clipPath);
        switch (_stackSwitchMode)
        {
        case ElaWindowType::None:
        {
            break;
        }
        case ElaWindowType::Popup:
        {
            painter.drawPixmap(QRect(0, _pPopupAnimationYOffset, width(), height()), _targetStackPix);
            break;
        }
        case ElaWindowType::Scale:
        {
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            painter.setOpacity(_pScaleAnimationPixOpacity);
            painter.translate(rect().center());
            painter.scale(_pScaleAnimationRatio, _pScaleAnimationRatio);
            painter.translate(-rect().center());
            painter.drawPixmap(rect(), _isDrawNewPix ? _targetStackPix : _currentStackPix);
            break;
        }
        case ElaWindowType::Flip:
        {
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            QTransform transform;
            transform.translate(rect().center().x(), 0);
            if (abs(_pFlipAnimationRatio) >= 90)
            {
                transform.rotate(-180 + _pFlipAnimationRatio, Qt::YAxis);
            }
            else
            {
                transform.rotate(_pFlipAnimationRatio, Qt::YAxis);
            }
            transform.translate(-rect().center().x(), 0);
            painter.setTransform(transform);
            if (abs(_pFlipAnimationRatio) >= 90)
            {
                painter.drawPixmap(rect(), _targetStackPix);
            }
            else
            {
                painter.drawPixmap(rect(), _currentStackPix);
            }
            break;
        }
        }
    }
    painter.restore();
}

void ElaCentralStackedWidget::_getCurrentStackPix()
{
    _targetStackPix = QPixmap();
    bool isTransparent = _isTransparent;
    _isTransparent = true;
    currentWidget()->setVisible(true);
    _currentStackPix = this->grab(rect());
    currentWidget()->setVisible(false);
    _isTransparent = isTransparent;
}

void ElaCentralStackedWidget::_getTargetStackPix()
{
    _targetStackPix = QPixmap();
    bool isTransparent = _isTransparent;
    _isTransparent = true;
    _targetStackPix = this->grab(rect());
    _isTransparent = isTransparent;
}
