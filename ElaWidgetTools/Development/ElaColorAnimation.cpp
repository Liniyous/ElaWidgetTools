#include "ElaColorAnimation.h"

#include "ElaTheme.h"

#include <QAbstractScrollArea>
#include <QApplication>
#include <QEvent>
#include <QStyle>
#include <QWidget>

QElapsedTimer ElaColorAnimation::_themeClock;
qint64 ElaColorAnimation::_themeChangeTime{0};
quint64 ElaColorAnimation::_themeRevision{0};

QColor ElaColorAnimation::getColor(const QWidget* widget, const QColor& target, const QByteArray& role, const QModelIndex& index, const QObject* subject)
{
    return getColor(widget, target, role, index, subject, false);
}

QColor ElaColorAnimation::getColor(const QWidget* widget, const QColor& target, const QByteArray& role, const QModelIndex& index, const QObject* subject, bool isImmediate)
{
    if (!widget || !target.isValid())
    {
        return target;
    }
    return _getAnimation(widget)->_getColor({role, QPersistentModelIndex(index), subject}, target, isImmediate);
}

bool ElaColorAnimation::getIsAnimationEnabled(const QWidget* widget)
{
    return widget && QApplication::isEffectEnabled(Qt::UI_General) && widget->style()->styleHint(QStyle::SH_Widget_Animate, nullptr, widget);
}

void ElaColorAnimation::beginThemeTransition()
{
    if (!_themeClock.isValid())
    {
        _themeClock.start();
    }
    _themeChangeTime = _themeClock.elapsed();
    ++_themeRevision;
}

int ElaColorAnimation::duration() const
{
    // 各颜色独立记录起点 共用 Qt 动画时钟 最后一个颜色完成后停止
    return -1;
}

bool ElaColorAnimation::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == _widget)
    {
        switch (event->type())
        {
        case QEvent::Hide:
        case QEvent::EnabledChange:
        case QEvent::StyleChange:
        {
            stop();
            _colors.clear();
            _updateWidget();
            break;
        }
        case QEvent::Polish:
        case QEvent::ChildAdded:
        {
            _syncFocusProxy();
            break;
        }
        case QEvent::Enter:
        case QEvent::Leave:
        case QEvent::FocusIn:
        case QEvent::FocusOut:
        {
            _updateWidget();
            break;
        }
        default:
        {
            break;
        }
        }
    }
    else if (watched && watched == _focusProxy)
    {
        if (event->type() == QEvent::FocusIn || event->type() == QEvent::FocusOut)
        {
            _updateWidget();
        }
    }
    return QAbstractAnimation::eventFilter(watched, event);
}

void ElaColorAnimation::updateCurrentTime(int currentTime)
{
    bool isRunning = false;
    const bool isAnimationEnabled = _widget && _widget->isEnabled() && getIsAnimationEnabled(_widget);
    for (auto it = _colors.begin(); it != _colors.end();)
    {
        ColorState& color = it.value();
        if ((color.isModelItem && !it.key().index.isValid()) || (it.key().subject && !color.subject))
        {
            it = _colors.erase(it);
            continue;
        }
        if (color.isRunning)
        {
            const qint64 time = color.isThemeTransition ? _themeClock.elapsed() : currentTime;
            _updateColor(color, isAnimationEnabled ? time : color.startTime + _transitionDuration);
            isRunning = isRunning || color.isRunning;
        }
        ++it;
    }
    _updateWidget();
    if (!isRunning)
    {
        stop();
    }
}

bool ElaColorAnimation::ColorKey::operator==(const ColorKey& other) const
{
    return role == other.role && index == other.index && subject == other.subject;
}

ElaColorAnimation::ElaColorAnimation(QWidget* widget)
    : QAbstractAnimation(widget)
{
    _widget = widget;
    widget->installEventFilter(this);
    _syncFocusProxy();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this]() {
        bool isRunning = false;
        for (ColorState& color: _colors)
        {
            // 主题通知中的同步绘制可能已经启动新过渡 仅停止旧版本
            if (color.themeRevision != _themeRevision)
            {
                color.isRunning = false;
            }
            isRunning = isRunning || color.isRunning;
        }
        if (!isRunning)
        {
            stop();
        }
        _updateWidget();
    });
}

ElaColorAnimation* ElaColorAnimation::_getAnimation(const QWidget* widget)
{
    auto* animation = widget->findChild<ElaColorAnimation*>(QString(), Qt::FindDirectChildrenOnly);
    if (!animation)
    {
        animation = new ElaColorAnimation(const_cast<QWidget*>(widget));
    }
    return animation;
}

QColor ElaColorAnimation::_getColor(const ColorKey& key, const QColor& target, bool isImmediate)
{
    auto it = _colors.find(key);
    if (it != _colors.end() && key.subject && !it.value().subject)
    {
        _colors.erase(it);
        it = _colors.end();
    }
    if (it == _colors.end())
    {
        _pruneColors();
        ColorState color;
        color.current = target;
        color.target = target;
        color.themeRevision = _themeRevision;
        color.subject = key.subject;
        color.isModelItem = key.index.isValid();
        it = _colors.insert(key, color);
    }
    ColorState& color = it.value();
    color.lastUse = ++_useSerial;
    const bool isThemeChanged = color.themeRevision != _themeRevision;
    color.themeRevision = _themeRevision;
    const bool isThemeFinished = isThemeChanged && _themeClock.elapsed() - _themeChangeTime >= _transitionDuration;
    if (!_widget->isVisible() || !_widget->isEnabled() || !getIsAnimationEnabled(_widget) || isThemeFinished || (isImmediate && !isThemeChanged))
    {
        color.current = target;
        color.target = target;
        color.isRunning = false;
    }
    else if (target != color.target || (isThemeChanged && color.current != target))
    {
        color.start = color.current;
        color.target = target;
        color.isThemeTransition = isThemeChanged;
        color.startTime = isThemeChanged ? _themeChangeTime : state() == Stopped ? 0
                                                                                 : currentTime();
        color.isRunning = true;
        _updateColor(color, isThemeChanged ? _themeClock.elapsed() : color.startTime);
        if (color.isRunning && state() == Stopped)
        {
            start();
        }
    }
    else if (color.isRunning && color.isThemeTransition)
    {
        _updateColor(color, _themeClock.elapsed());
    }
    return color.current;
}

void ElaColorAnimation::_updateColor(ColorState& color, qint64 currentTime)
{
    const qreal progress = qBound<qreal>(0, (currentTime - color.startTime) / static_cast<qreal>(_transitionDuration), 1);
    // 在预乘透明度空间插值 保证不同透明度的深浅色过渡时叠加亮度严格单调不跳变
    const qreal startAlpha = color.start.alphaF();
    const qreal targetAlpha = color.target.alphaF();
    const qreal currentAlpha = startAlpha + (targetAlpha - startAlpha) * progress;
    if (currentAlpha <= 0.0001)
    {
        color.current = QColor(0, 0, 0, 0);
    }
    else
    {
        const qreal startRed = color.start.redF() * startAlpha;
        const qreal startGreen = color.start.greenF() * startAlpha;
        const qreal startBlue = color.start.blueF() * startAlpha;
        const qreal targetRed = color.target.redF() * targetAlpha;
        const qreal targetGreen = color.target.greenF() * targetAlpha;
        const qreal targetBlue = color.target.blueF() * targetAlpha;
        const qreal invAlpha = 1.0 / currentAlpha;
        color.current = QColor::fromRgbF(
            qBound<qreal>(0.0, (startRed + (targetRed - startRed) * progress) * invAlpha, 1.0),
            qBound<qreal>(0.0, (startGreen + (targetGreen - startGreen) * progress) * invAlpha, 1.0),
            qBound<qreal>(0.0, (startBlue + (targetBlue - startBlue) * progress) * invAlpha, 1.0),
            qBound<qreal>(0.0, currentAlpha, 1.0));
    }
    color.isRunning = progress < 1;
    if (!color.isRunning)
    {
        color.current = color.target;
    }
}

void ElaColorAnimation::_syncFocusProxy()
{
    if (!_widget)
    {
        return;
    }
    QWidget* proxy = _widget->focusProxy();
    if (_focusProxy == proxy)
    {
        return;
    }
    if (_focusProxy)
    {
        _focusProxy->removeEventFilter(this);
    }
    _focusProxy = proxy;
    if (_focusProxy && _focusProxy != _widget)
    {
        _focusProxy->installEventFilter(this);
    }
}

void ElaColorAnimation::_pruneColors()
{
    // 长列表仅保留近期绘制的颜色 不随滚动无限持有模型索引
    if (_colors.size() < 4096)
    {
        return;
    }
    const quint64 oldestUse = _useSerial > 2048 ? _useSerial - 2048 : 0;
    for (auto it = _colors.begin(); it != _colors.end();)
    {
        if (it.value().lastUse <= oldestUse || (it.value().isModelItem && !it.key().index.isValid()) || (it.key().subject && !it.value().subject))
        {
            it = _colors.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void ElaColorAnimation::_updateWidget()
{
    if (!_widget)
    {
        return;
    }
    if (auto* scrollArea = qobject_cast<QAbstractScrollArea*>(_widget.data()))
    {
        scrollArea->viewport()->update();
    }
    _widget->update();
}
