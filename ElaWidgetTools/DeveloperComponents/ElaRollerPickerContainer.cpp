#include "ElaRollerPickerContainer.h"
#include "ElaRoller.h"
#include "ElaTheme.h"
#include <QMouseEvent>
#include <QPainter>
ElaRollerPickerContainer::ElaRollerPickerContainer(QWidget* parent)
    : QWidget{parent}
{
    _pAnimationPixOffsetY = 0;
    _pButtonAreaHeight = 38;
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(8, 8, 8, 6 + _pButtonAreaHeight);
    setObjectName("ElaCalendarPickerContainer");
    setStyleSheet("#ElaCalendarPickerContainer{background-color:transparent}");

    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
}

ElaRollerPickerContainer::~ElaRollerPickerContainer()
{
}

void ElaRollerPickerContainer::doPickerAnimation()
{
    _handleSaveOrReset(true);
    if (!_animationPix.isNull())
    {
        _animationPix = QPixmap();
    }
    _animationPix = this->grab(rect());
    QPropertyAnimation* offsetAnimation = new QPropertyAnimation(this, "pAnimationPixOffsetY");
    connect(offsetAnimation, &QPropertyAnimation::finished, this, [=]() {
        _animationPix = QPixmap();
        update();
    });
    connect(offsetAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    offsetAnimation->setEasingCurve(QEasingCurve::OutCubic);
    offsetAnimation->setDuration(175);
    offsetAnimation->setStartValue(70);
    offsetAnimation->setEndValue(0);
    offsetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaRollerPickerContainer::mouseMoveEvent(QMouseEvent* event)
{
    auto currentPos = event->pos();
    if (_overButtonRect.contains(currentPos))
    {
        _isOverButtonHover = true;
        _isCancelButtonHover = false;
        update();
    }
    else if (_cancelButtonRect.contains(currentPos))
    {
        _isCancelButtonHover = true;
        _isOverButtonHover = false;
        update();
    }
    else
    {
        if (_isCancelButtonHover || _isOverButtonHover)
        {
            _isCancelButtonHover = false;
            _isOverButtonHover = false;
            update();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void ElaRollerPickerContainer::mouseReleaseEvent(QMouseEvent* event)
{
    auto currentPos = event->pos();
    if (_overButtonRect.contains(currentPos))
    {
        _isOverButtonClicked = true;
        Q_EMIT overButtonClicked();
        hide();
    }
    else if (_cancelButtonRect.contains(currentPos))
    {
        _handleSaveOrReset(false);
        Q_EMIT cancelButtonClicked();
        hide();
    }
    QWidget::mouseReleaseEvent(event);
}

void ElaRollerPickerContainer::leaveEvent(QEvent* event)
{
    _isOverButtonHover = false;
    _isCancelButtonHover = false;
    update();
    QWidget::leaveEvent(event);
}

void ElaRollerPickerContainer::hideEvent(QHideEvent* event)
{
    if (_isOverButtonClicked)
    {
        _isOverButtonClicked = false;
    }
    else
    {
        _handleSaveOrReset(false);
    }
    QWidget::hideEvent(event);
}

void ElaRollerPickerContainer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    if (!_animationPix.isNull())
    {
        painter.setClipRect(QRect(0, _pAnimationPixOffsetY, width(), height() - 2 * _pAnimationPixOffsetY));
        painter.drawPixmap(rect(), _animationPix);
    }
    else
    {
        eTheme->drawEffectShadow(&painter, rect(), 6, 5);
        painter.setPen(ElaThemeColor(_themeMode, PopupBorder));
        painter.setBrush(ElaThemeColor(_themeMode, PopupBase));
        QRect foregroundRect(6, 6, rect().width() - 2 * 6, rect().height() - 2 * 6);
        painter.drawRoundedRect(foregroundRect, 8, 8);
        // 纵向分割线
        painter.setPen(ElaThemeColor(_themeMode, BasicBorder));
        int rollerXOffset = 8;
        for (int i = 0; i < _rollerList.count(); i++)
        {
            rollerXOffset += _rollerList[i]->width();
            if (i != _rollerList.count() - 1)
            {
                painter.setPen(ElaThemeColor(_themeMode, BasicBorder));
                painter.drawLine(rollerXOffset, foregroundRect.y(), rollerXOffset, foregroundRect.bottom() - _pButtonAreaHeight);
            }
        }
        // 横向分割线
        painter.drawLine(foregroundRect.x(), foregroundRect.bottom() - _pButtonAreaHeight, foregroundRect.right(), foregroundRect.bottom() - _pButtonAreaHeight);
        // 按钮区域绘制
        _overButtonRect = QRect(foregroundRect.x() + _buttonMargin, foregroundRect.bottom() - _pButtonAreaHeight + _buttonMargin, (foregroundRect.width() - 2 * _buttonMargin - _buttonSpacing) / 2, _pButtonAreaHeight - 2 * _buttonMargin + 1);
        _cancelButtonRect = QRect(_overButtonRect.right() + _buttonSpacing, _overButtonRect.y(), _overButtonRect.width(), _overButtonRect.height());
        // 按钮覆盖绘制
        painter.setPen(Qt::NoPen);
        painter.setBrush(ElaThemeColor(_themeMode, BasicHover));
        if (_isOverButtonHover)
        {
            painter.drawRoundedRect(_overButtonRect, 5, 5);
        }
        else if (_isCancelButtonHover)
        {
            painter.drawRoundedRect(_cancelButtonRect, 5, 5);
        }
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(17);
        painter.setFont(iconFont);
        painter.setPen(ElaThemeColor(_themeMode, BasicText));
        // 确定
        painter.drawText(_overButtonRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::Check));
        // 取消
        painter.drawText(_cancelButtonRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::Xmark));
    }
    painter.restore();
}

void ElaRollerPickerContainer::_handleSaveOrReset(bool isSave)
{
    if (isSave)
    {
        // 保存历史数据
        _historyIndexList.clear();
        for (auto roller: _rollerList)
        {
            _historyIndexList.append(roller->getCurrentIndex());
        }
    }
    else
    {
        // 恢复数据
        for (int i = 0; i < _rollerList.count(); i++)
        {
            auto roller = _rollerList[i];
            roller->setCurrentIndex(_historyIndexList[i]);
        }
    }
}
