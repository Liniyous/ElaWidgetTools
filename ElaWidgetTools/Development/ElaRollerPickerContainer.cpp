#include "ElaRollerPickerContainer.h"

#include "ElaApplication.h"
#include "ElaColorAnimation.h"
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
    setContentsMargins(eTheme->getShadowMargins() + QMargins(2, 2, 2, _pButtonAreaHeight));
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
        const QMargins margins = eTheme->getShadowMargins();
        eTheme->drawEffectShadow(&painter, rect(), margins, 8);
        painter.setPen(ElaThemeColor(_themeMode, BasicBorder));
        painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(_themeMode, PopupBase), "Background"));
        const QRect foregroundRect = rect().marginsRemoved(margins);
        painter.drawRoundedRect(foregroundRect, 8, 8);
        // 纵向分割线
        painter.setPen(ElaThemeColor(_themeMode, BasicBorder));
        int rollerXOffset = margins.left() + 2;
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
        painter.setBrush(ElaColorAnimation::getColor(this, _isOverButtonHover ? ElaThemeColor(_themeMode, BasicHoverAlpha) : QColor(Qt::transparent), "Confirm"));
        painter.drawRoundedRect(_overButtonRect, 5, 5);
        painter.setBrush(ElaColorAnimation::getColor(this, _isCancelButtonHover ? ElaThemeColor(_themeMode, BasicHoverAlpha) : QColor(Qt::transparent), "Cancel"));
        painter.drawRoundedRect(_cancelButtonRect, 5, 5);
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(eApp->getFontPixelSize() + 4);
        painter.setFont(iconFont);
        painter.setPen(ElaThemeColor(_themeMode, BasicText));
        // 确定
        painter.drawText(_overButtonRect, Qt::AlignCenter, QChar(ElaIconType::Check));
        // 取消
        painter.drawText(_cancelButtonRect, Qt::AlignCenter, QChar(ElaIconType::Xmark));
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
