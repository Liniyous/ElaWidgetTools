#include "ElaMessageBarPrivate.h"

#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTimer>

#include "ElaMessageBar.h"
QMap<ElaMessageBarType::PositionPolicy, QList<ElaMessageBar*>*> ElaMessageBarPrivate::_messageBarActiveMap;
ElaMessageBarPrivate::ElaMessageBarPrivate(QObject* parent)
    : QObject{parent}
{
    setProperty("MessageBarClosedY", 0);
    setProperty("MessageBarFinishY", 0);
}

ElaMessageBarPrivate::~ElaMessageBarPrivate()
{
}

void ElaMessageBarPrivate::_onCloseButtonClicked()
{
    Q_Q(ElaMessageBar);
    if (_isCloseAnimationStart)
    {
        return;
    }
    _isCloseAnimationStart = true;
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(q->graphicsEffect(), "opacity");
    connect(opacityAnimation, &QPropertyAnimation::finished, q, [=]() { q->deleteLater(); });
    opacityAnimation->setStartValue(dynamic_cast<QGraphicsOpacityEffect*>(q->graphicsEffect())->opacity());
    opacityAnimation->setEndValue(0);
    opacityAnimation->setDuration(220);
    opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    _isNormalDisplay = false;
    _updateActiveMap(false);
    Q_EMIT messageBarClosed(_policy, _messageBarIndex);
}

void ElaMessageBarPrivate::onOtherMessageBarClosed(ElaMessageBarType::PositionPolicy policy, int messageBarIndex)
{
    // ElaMessageBarType::PositionPolicy policy = data.value("MessageBarPolicy").value<ElaMessageBarType::PositionPolicy>();
    // int index = data.value("MessageBarIndex").toUInt();
    if (_policy == policy && !_isCloseAnimationStart && _messageBarIndex > messageBarIndex)
    {
        _messageBarIndex -= 1;
        if (!_isMessageBarStartAnimationFinished)
        {
            _isMessageBarEventAnimationInStartAnimation = true;
            return;
        }
        _messageBarEventAnimation();
    }
}

void ElaMessageBarPrivate::_messageBarStartAnimation(int displayMsec)
{
    Q_Q(ElaMessageBar);
    q->show();
    QFont font = q->font();
    font.setPixelSize(16);
    font.setWeight(QFont::Bold);
    q->setFont(font);
    int titleWidth = q->fontMetrics().horizontalAdvance(_title);
    font.setPixelSize(14);
    font.setWeight(QFont::Medium);
    q->setFont(font);
    int textWidth = q->fontMetrics().horizontalAdvance(_text);
    int fixedWidth = _closeButtonLeftRightMargin + _leftPadding + _titleLeftSpacing + _textLeftSpacing + _closeButtonWidth + titleWidth + textWidth + 2 * _shadowBorderWidth;
    q->setFixedWidth(fixedWidth > 500 ? 500 : fixedWidth);
    _updateActiveMap(true); // 计算坐标前增加
    int startX = 0;
    int startY = 0;
    int endX = 0;
    int endY = 0;
    _calculatePos(startX, startY, endX, endY);
    // 滑入动画
    QPropertyAnimation* barPosAnimation = new QPropertyAnimation(q, "pos");
    connect(barPosAnimation, &QPropertyAnimation::finished, q, [=]() {
        _isNormalDisplay = true;
        _isMessageBarStartAnimationFinished = true;
        if(_isMessageBarEventAnimationInStartAnimation)
        {
            _messageBarEventAnimation();
        }
        QTimer::singleShot(displayMsec, q, [=]() {
            _isCloseAnimationStart = true;
            if (!_isMessageBarEventAnimationStart)
            {
                _messageBarFinshAnimation();
            }
        }); });
    switch (_policy)
    {
    case ElaMessageBarType::Top:
    case ElaMessageBarType::Bottom:
    {
        barPosAnimation->setDuration(250);
        break;
    }
    case ElaMessageBarType::Left:
    case ElaMessageBarType::Right:
    case ElaMessageBarType::TopRight:
    case ElaMessageBarType::TopLeft:
    case ElaMessageBarType::BottomRight:
    case ElaMessageBarType::BottomLeft:
    {
        barPosAnimation->setDuration(450);
        break;
    }
    }
    barPosAnimation->setStartValue(QPoint(startX, startY));
    barPosAnimation->setEndValue(QPoint(endX, endY));
    barPosAnimation->setEasingCurve(QEasingCurve::InOutSine);
    barPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaMessageBarPrivate::_messageBarFinshAnimation()
{
    Q_Q(ElaMessageBar);
    QPropertyAnimation* barFinishedOpacityAnimation = new QPropertyAnimation(q->graphicsEffect(), "opacity");
    connect(barFinishedOpacityAnimation, &QPropertyAnimation::finished, this, [=]() {
        q->deleteLater();
    });
    barFinishedOpacityAnimation->setDuration(300);
    barFinishedOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    barFinishedOpacityAnimation->setStartValue(1);
    barFinishedOpacityAnimation->setEndValue(0);
    barFinishedOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    _updateActiveMap(false);
    Q_EMIT messageBarClosed(_policy, _messageBarIndex);
}

void ElaMessageBarPrivate::_messageBarEventAnimation()
{
    Q_Q(ElaMessageBar);
    _isMessageBarEventAnimationStart = true;
    QPropertyAnimation* closePosAnimation = new QPropertyAnimation(this, "MessageBarClosedY");
    connect(closePosAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { q->move(q->pos().x(), value.toUInt()); });
    connect(closePosAnimation, &QPropertyAnimation::finished, this, [=]() {
                if (_isCloseAnimationStart)
                {
                    _messageBarFinshAnimation();
                }
                else
                {
                    _isMessageBarEventAnimationStart = false;
                } });
    closePosAnimation->setEasingCurve(QEasingCurve::InOutSine);
    closePosAnimation->setDuration(200);
    closePosAnimation->setStartValue(q->pos().y());
    int minimumHeightTotal = _calculateMinimumHeightTotal(true);
    switch (_policy)
    {
    case ElaMessageBarType::Top:
    {
        closePosAnimation->setEndValue(minimumHeightTotal + _messageBarSpacing * (_messageBarIndex - 1) + _messageBarVerticalTopMargin);
        break;
    }
    case ElaMessageBarType::Left:
    {
        closePosAnimation->setEndValue(minimumHeightTotal + _messageBarSpacing * (_messageBarIndex - 1) + q->parentWidget()->height() / 2);
        break;
    }
    case ElaMessageBarType::Bottom:
    {
        closePosAnimation->setEndValue(q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - _messageBarSpacing * (_messageBarIndex - 1) - _messageBarVerticalBottomMargin);
        break;
    }
    case ElaMessageBarType::Right:
    {
        closePosAnimation->setEndValue(minimumHeightTotal + _messageBarSpacing * (_messageBarIndex - 1) + q->parentWidget()->height() / 2);
        break;
    }
    case ElaMessageBarType::TopRight:
    {
        closePosAnimation->setEndValue(minimumHeightTotal + _messageBarSpacing * (_messageBarIndex - 1) + _messageBarVerticalTopMargin);
        break;
    }
    case ElaMessageBarType::TopLeft:
    {
        closePosAnimation->setEndValue(minimumHeightTotal + _messageBarSpacing * (_messageBarIndex - 1) + _messageBarVerticalTopMargin);
        break;
    }
    case ElaMessageBarType::BottomRight:
    {
        closePosAnimation->setEndValue(q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - _messageBarSpacing * (_messageBarIndex - 1) - _messageBarVerticalBottomMargin);
        break;
    }
    case ElaMessageBarType::BottomLeft:
    {
        closePosAnimation->setEndValue(q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - _messageBarSpacing * (_messageBarIndex - 1) - _messageBarVerticalBottomMargin);
        break;
    }
    }
    closePosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaMessageBarPrivate::_updateActiveMap(bool isActive)
{
    Q_Q(ElaMessageBar);
    if (isActive)
    {
        if (_messageBarActiveMap.contains(_policy))
        {
            _messageBarActiveMap[_policy]->append(q);
        }
        else
        {
            QList<ElaMessageBar*>* messageBarList = new QList<ElaMessageBar*>();
            messageBarList->append(q);
            _messageBarActiveMap.insert(_policy, messageBarList);
        }
        _messageBarIndex = _messageBarActiveMap[_policy]->count();
    }
    else
    {
        if (_messageBarActiveMap.contains(_policy))
        {
            if (_messageBarActiveMap[_policy]->count() > 0)
            {
                _messageBarActiveMap[_policy]->removeOne(q);
            }
        }
    }
}

void ElaMessageBarPrivate::_calculatePos(int& startX, int& startY, int& endX, int& endY)
{
    Q_Q(ElaMessageBar);
    int minimumHeightTotal = _calculateMinimumHeightTotal();
    switch (_policy)
    {
    case ElaMessageBarType::Top:
    {
        // 25动画距离
        startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
        startY = minimumHeightTotal + _messageBarSpacing * (_messageBarIndex - 1) + _messageBarVerticalTopMargin - 25;
        endX = startX;
        endY = minimumHeightTotal + _messageBarSpacing * (_messageBarIndex - 1) + _messageBarVerticalTopMargin;
        break;
    }
    case ElaMessageBarType::Left:
    {
        startX = -q->minimumWidth();
        startY = minimumHeightTotal + _messageBarSpacing * (_messageBarIndex - 1) + q->parentWidget()->height() / 2;
        endX = _messageBarHorizontalMargin;
        endY = startY;
        break;
    }
    case ElaMessageBarType::Bottom:
    {
        startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
        startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - _messageBarSpacing * (_messageBarIndex - 1) - _messageBarVerticalBottomMargin - 25;
        endX = startX;
        endY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - _messageBarSpacing * (_messageBarIndex - 1) - _messageBarVerticalBottomMargin;
        break;
    }
    case ElaMessageBarType::Right:
    {
        startX = q->parentWidget()->width();
        startY = minimumHeightTotal + _messageBarSpacing * (_messageBarIndex - 1) + q->parentWidget()->height() / 2;
        endX = q->parentWidget()->width() - q->minimumWidth() - _messageBarHorizontalMargin;
        endY = startY;
        break;
    }
    case ElaMessageBarType::TopRight:
    {
        startX = q->parentWidget()->width();
        startY = minimumHeightTotal + _messageBarSpacing * (_messageBarIndex - 1) + _messageBarVerticalTopMargin;
        endX = q->parentWidget()->width() - q->minimumWidth() - _messageBarHorizontalMargin;
        endY = startY;
        break;
    }
    case ElaMessageBarType::TopLeft:
    {
        startX = -q->minimumWidth();
        startY = minimumHeightTotal + _messageBarSpacing * (_messageBarIndex - 1) + _messageBarVerticalTopMargin;
        endX = _messageBarHorizontalMargin;
        endY = startY;
        break;
    }
    case ElaMessageBarType::BottomRight:
    {
        startX = q->parentWidget()->width();
        startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - _messageBarSpacing * (_messageBarIndex - 1) - _messageBarVerticalBottomMargin;
        endX = q->parentWidget()->width() - q->minimumWidth() - _messageBarHorizontalMargin;
        endY = startY;
        break;
    }
    case ElaMessageBarType::BottomLeft:
    {
        startX = -q->minimumWidth();
        startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - _messageBarSpacing * (_messageBarIndex - 1) - _messageBarVerticalBottomMargin;
        endX = _messageBarHorizontalMargin;
        endY = startY;
        break;
    }
    }
    if (endY < _messageBarVerticalTopMargin || endY > q->parentWidget()->height() - _messageBarVerticalBottomMargin - q->minimumHeight())
    {
        _updateActiveMap(false);
        q->deleteLater();
    }
}

int ElaMessageBarPrivate::_calculateMinimumHeightTotal(bool isJudgeIndex)
{
    Q_Q(ElaMessageBar);
    int minimumHeightTotal = 0;
    QList<ElaMessageBar*>* messageBarList = _messageBarActiveMap[_policy];
    if (isJudgeIndex)
    {
        for (auto messageBar : *messageBarList)
        {
            if (messageBar == q)
            {
                continue;
            }
            if (messageBar->d_ptr->_messageBarIndex < _messageBarIndex)
            {
                minimumHeightTotal += messageBar->minimumHeight();
            }
        }
    }
    else
    {
        for (auto messageBar : *messageBarList)
        {
            if (messageBar == q)
            {
                continue;
            }
            minimumHeightTotal += messageBar->minimumHeight();
        }
    }
    return minimumHeightTotal;
}

void ElaMessageBarPrivate::_drawSuccess(QPainter* painter)
{
    Q_Q(ElaMessageBar);
    painter->setBrush(QColor(0xE0, 0xF6, 0xDD));
    QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth, q->width() - 2 * _shadowBorderWidth, q->height() - 2 * _shadowBorderWidth);
    painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    // 图标绘制
    painter->save();
    painter->setPen(Qt::white);
    QPainterPath textPath;
    textPath.addEllipse(QPoint(_leftPadding + 6, q->height() / 2), 9, 9);
    painter->setClipPath(textPath);
    painter->fillPath(textPath, QColor(0x11, 0x77, 0x10));
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(12);
    painter->setFont(iconFont);
    painter->drawText(_leftPadding, 0, q->width(), q->height(), Qt::AlignVCenter, QChar((unsigned short)ElaIconType::Check));
    painter->restore();
    // 文字颜色
    painter->setPen(QPen(Qt::black));
}

void ElaMessageBarPrivate::_drawWarning(QPainter* painter)
{
    Q_Q(ElaMessageBar);
    painter->setBrush(QColor(0x6B, 0x56, 0x27));
    QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth, q->width() - 2 * _shadowBorderWidth, q->height() - 2 * _shadowBorderWidth);
    painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    // 图标绘制
    // exclamation
    painter->save();
    painter->setPen(Qt::black);
    QPainterPath textPath;
    textPath.addEllipse(QPoint(_leftPadding + 6, q->height() / 2), 9, 9);
    painter->setClipPath(textPath);
    painter->fillPath(textPath, QColor(0xF8, 0xE2, 0x23));
    painter->drawText(_leftPadding + 4, 0, q->width(), q->height(), Qt::AlignVCenter, "!");
    painter->restore();
    // 文字颜色
    painter->setPen(QColor(0xFA, 0xFA, 0xFA));
}

void ElaMessageBarPrivate::_drawInformation(QPainter* painter)
{
    Q_Q(ElaMessageBar);
    painter->setBrush(QColor(0xF4, 0xF4, 0xF4));
    QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth, q->width() - 2 * _shadowBorderWidth, q->height() - 2 * _shadowBorderWidth);
    painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    // 图标绘制
    painter->save();
    painter->setPen(Qt::white);
    QPainterPath textPath;
    textPath.addEllipse(QPoint(_leftPadding + 6, q->height() / 2), 9, 9);
    painter->setClipPath(textPath);
    painter->fillPath(textPath, QColor(0x00, 0x66, 0xB4));
    painter->drawText(_leftPadding + 4, 0, q->width(), q->height(), Qt::AlignVCenter, "i");
    painter->restore();
    // 文字颜色
    painter->setPen(Qt::black);
}

void ElaMessageBarPrivate::_drawError(QPainter* painter)
{
    Q_Q(ElaMessageBar);
    painter->setBrush(QColor(0xFE, 0xE7, 0xEA));
    QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth, q->width() - 2 * _shadowBorderWidth, q->height() - 2 * _shadowBorderWidth);
    painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    // 图标绘制
    painter->save();
    painter->setPen(Qt::white);
    QPainterPath textPath;
    textPath.addEllipse(QPoint(_leftPadding + 6, q->height() / 2), 9, 9);
    painter->setClipPath(textPath);
    painter->fillPath(textPath, QColor(0xBA, 0x2D, 0x20));
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(13);
    painter->setFont(iconFont);
    painter->drawText(_leftPadding + 1, 0, q->width(), q->height(), Qt::AlignVCenter, QChar((unsigned short)ElaIconType::Xmark));
    painter->restore();
    // 文字颜色
    painter->setPen(Qt::black);
}
