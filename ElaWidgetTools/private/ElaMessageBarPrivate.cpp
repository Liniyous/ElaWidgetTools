#include "ElaMessageBarPrivate.h"

#include "ElaIconButton.h"
#include "ElaMessageBar.h"
#include <QDateTime>
#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTimer>

QMap<ElaMessageBarType::PositionPolicy, QList<ElaMessageBar*>*> _messageBarActiveMap;
ElaMessageBarPrivate::ElaMessageBarPrivate(QObject* parent)
    : QObject{parent}
{
    setProperty("MessageBarClosedY", 0);
    setProperty("MessageBarFinishY", 0);
    _pTimePercent = 100;
    _createTime = QDateTime::currentMSecsSinceEpoch();
}

ElaMessageBarPrivate::~ElaMessageBarPrivate()
{
}

void ElaMessageBarPrivate::onOtherMessageBarEnd()
{
    Q_Q(ElaMessageBar);
    qreal targetPosY = _calculateTargetPosY();
    QPropertyAnimation* closePosAnimation = new QPropertyAnimation(this, "MessageBarClosedY");
    connect(closePosAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        q->move(q->pos().x(), value.toUInt());
    });
    closePosAnimation->setEasingCurve(QEasingCurve::OutCubic);
    closePosAnimation->setDuration(220);
    closePosAnimation->setStartValue(q->pos().y());
    closePosAnimation->setEndValue(targetPosY);
    closePosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaMessageBarPrivate::messageBarEnd()
{
    Q_Q(ElaMessageBar);
    _closeButton->setEnabled(false);
    _updateActiveMap(false);
    QPropertyAnimation* barFinishedOpacityAnimation = new QPropertyAnimation(this, "pOpacity");
    connect(barFinishedOpacityAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
        _closeButton->setOpacity(_pOpacity);
        q->update();
    });
    connect(barFinishedOpacityAnimation, &QPropertyAnimation::finished, this, [=]() {
        q->deleteLater();
    });
    barFinishedOpacityAnimation->setDuration(300);
    barFinishedOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    barFinishedOpacityAnimation->setStartValue(1);
    barFinishedOpacityAnimation->setEndValue(0);
    barFinishedOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    // 通知同类型的其他MessageBar
    for (const auto messageBar: *_messageBarActiveMap[_policy])
    {
        if (messageBar->d_ptr->_isNormalDisplay)
        {
            messageBar->d_ptr->onOtherMessageBarEnd();
        }
        else
        {
            messageBar->d_ptr->_isOtherMessageBarEnd = true;
        }
    }
}

void ElaMessageBarPrivate::_messageBarCreate(int displayMsec)
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
    int startX = 0;
    int startY = 0;
    int endX = 0;
    int endY = 0;
    _updateActiveMap(true);
    _calculateInitialPos(startX, startY, endX, endY);
    // 滑入动画
    QPropertyAnimation* barPosAnimation = new QPropertyAnimation(q, "pos");
    connect(barPosAnimation, &QPropertyAnimation::finished, q, [=]() {
        _isNormalDisplay = true;
        if (_isOtherMessageBarEnd)
        {
            onOtherMessageBarEnd();
        }
        QTimer::singleShot(displayMsec, q, [=]() {
            messageBarEnd();
        });
        QPropertyAnimation* timePercentAnimation = new QPropertyAnimation(this, "pTimePercent");
        connect(timePercentAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            q->update();
        });
        timePercentAnimation->setStartValue(100);
        timePercentAnimation->setEndValue(0);
        timePercentAnimation->setEasingCurve(QEasingCurve::Linear);
        timePercentAnimation->setDuration(displayMsec);
        timePercentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    });
    switch (_policy)
    {
    case ElaMessageBarType::Top:
    case ElaMessageBarType::Bottom:
    {
        barPosAnimation->setDuration(250);
        break;
    }
    default:
    {
        barPosAnimation->setDuration(350);
        break;
    }
    }
    barPosAnimation->setStartValue(QPoint(startX, startY));
    barPosAnimation->setEndValue(QPoint(endX, endY));
    barPosAnimation->setEasingCurve(QEasingCurve::InOutSine);
    barPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaMessageBarPrivate::_calculateInitialPos(int& startX, int& startY, int& endX, int& endY)
{
    Q_Q(ElaMessageBar);
    QList<int> resultList = _getOtherMessageBarTotalData();
    int minimumHeightTotal = resultList[0];
    int indexLessCount = resultList[1];
    int lastEndY = endY;
    switch (_policy)
    {
    case ElaMessageBarType::Top:
    {
        // 25动画距离
        startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
        startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin - 25;
        endX = startX;
        endY = minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin;
        break;
    }
    case ElaMessageBarType::Left:
    {
        startX = -q->minimumWidth();
        startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + q->parentWidget()->height() / 2;
        endX = _messageBarHorizontalMargin;
        endY = startY;
        break;
    }
    case ElaMessageBarType::Bottom:
    {
        startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
        startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin - 25;
        endX = startX;
        endY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin;
        break;
    }
    case ElaMessageBarType::Right:
    {
        startX = q->parentWidget()->width();
        startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + q->parentWidget()->height() / 2;
        endX = q->parentWidget()->width() - q->minimumWidth() - _messageBarHorizontalMargin;
        endY = startY;
        break;
    }
    case ElaMessageBarType::TopRight:
    {
        startX = q->parentWidget()->width();
        startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin;
        endX = q->parentWidget()->width() - q->minimumWidth() - _messageBarHorizontalMargin;
        endY = startY;
        break;
    }
    case ElaMessageBarType::TopLeft:
    {
        startX = -q->minimumWidth();
        startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin;
        endX = _messageBarHorizontalMargin;
        endY = startY;
        break;
    }
    case ElaMessageBarType::BottomRight:
    {
        startX = q->parentWidget()->width();
        startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin;
        endX = q->parentWidget()->width() - q->minimumWidth() - _messageBarHorizontalMargin;
        endY = startY;
        break;
    }
    case ElaMessageBarType::BottomLeft:
    {
        startX = -q->minimumWidth();
        startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin;
        endX = _messageBarHorizontalMargin;
        endY = startY;
        break;
    }
    }
    if (endY == lastEndY)
    {
        return;
    }
    if (endY < _messageBarVerticalTopMargin || endY > q->parentWidget()->height() - _messageBarVerticalBottomMargin - q->minimumHeight())
    {
        (*_messageBarActiveMap[_policy])[0]->d_ptr->messageBarEnd();
        _calculateInitialPos(startX, startY, endX, endY);
    }
}

QList<int> ElaMessageBarPrivate::_getOtherMessageBarTotalData()
{
    Q_Q(ElaMessageBar);
    QList<int> resultList;
    int minimumHeightTotal = 0;
    int indexLessCount = 0;
    QList<ElaMessageBar*>* messageBarList = _messageBarActiveMap[_policy];
    for (const auto messageBar: *messageBarList)
    {
        if (messageBar == q)
        {
            continue;
        }
        if (_judgeCreateOrder(messageBar))
        {
            indexLessCount++;
            minimumHeightTotal += messageBar->minimumHeight();
        }
    }
    resultList.append(minimumHeightTotal);
    resultList.append(indexLessCount);
    return resultList;
}

qreal ElaMessageBarPrivate::_calculateTargetPosY()
{
    Q_Q(ElaMessageBar);
    QList<int> resultList = _getOtherMessageBarTotalData();
    int minimumHeightTotal = resultList[0];
    int indexLessCount = resultList[1];
    switch (_policy)
    {
    case ElaMessageBarType::Top:
    case ElaMessageBarType::TopRight:
    case ElaMessageBarType::TopLeft:
    {
        return minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin;
    }
    case ElaMessageBarType::Left:
    case ElaMessageBarType::Right:
    {
        return minimumHeightTotal + _messageBarSpacing * indexLessCount + q->parentWidget()->height() / 2;
    }
    case ElaMessageBarType::Bottom:
    case ElaMessageBarType::BottomRight:
    case ElaMessageBarType::BottomLeft:
    {
        return q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal - _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin;
    }
    }
    return 0;
}

bool ElaMessageBarPrivate::_judgeCreateOrder(ElaMessageBar* otherMessageBar)
{
    if (otherMessageBar->d_ptr->_createTime < _createTime)
    {
        //otherMessageBar先创建
        return true;
    }
    else
    {
        return false;
    }
}

void ElaMessageBarPrivate::_updateActiveMap(bool isActive)
{
    Q_Q(ElaMessageBar);
    ElaMessageBarType::PositionPolicy policy = _policy;
    if (isActive)
    {
        if (_messageBarActiveMap.contains(policy))
        {
            _messageBarActiveMap[policy]->append(q);
        }
        else
        {
            QList<ElaMessageBar*>* messageBarList = new QList<ElaMessageBar*>();
            messageBarList->append(q);
            _messageBarActiveMap.insert(policy, messageBarList);
        }
    }
    else
    {
        if (_messageBarActiveMap.contains(policy))
        {
            if (_messageBarActiveMap[policy]->count() > 0)
            {
                _messageBarActiveMap[policy]->removeOne(q);
            }
        }
    }
}

void ElaMessageBarPrivate::_drawSuccess(QPainter* painter)
{
    Q_Q(ElaMessageBar);
    painter->save();
    painter->setBrush(QColor(0xE0, 0xF6, 0xDD));
    QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth, q->width() - 2 * _shadowBorderWidth, q->height() - 2 * _shadowBorderWidth);
    painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    // 图标绘制
    painter->setPen(Qt::white);
    QPainterPath textPath;
    textPath.addEllipse(QPoint(_leftPadding + 6, q->height() / 2), 9, 9);
    painter->setClipPath(textPath);
    painter->fillPath(textPath, QColor(0x11, 0x77, 0x10));
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(12);
    painter->setFont(iconFont);
    painter->drawText(_leftPadding, 0, q->width(), q->height(), Qt::AlignVCenter, QChar(ElaIconType::Check));
    // 时间进度条绘制
    QPainterPath clipPath;
    clipPath.addRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    painter->setClipPath(clipPath);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0x3C, 0x96, 0x4B));
    painter->drawRoundedRect(QRectF(foregroundRect.x(), foregroundRect.bottom() - _timePercentHeight, foregroundRect.width() * _pTimePercent / 100.0, _timePercentHeight + 1), 2, 2);
    painter->restore();
    // 文字颜色
    painter->setPen(QPen(Qt::black));
}

void ElaMessageBarPrivate::_drawWarning(QPainter* painter)
{
    Q_Q(ElaMessageBar);
    painter->save();
    painter->setBrush(QColor(0xFF, 0xF4, 0xCE));
    QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth, q->width() - 2 * _shadowBorderWidth, q->height() - 2 * _shadowBorderWidth);
    painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    // 图标绘制
    // exclamation
    painter->setPen(Qt::black);
    QPainterPath textPath;
    textPath.addEllipse(QPoint(_leftPadding + 6, q->height() / 2), 9, 9);
    painter->setClipPath(textPath);
    painter->fillPath(textPath, QColor(0xF8, 0xE2, 0x23));
    painter->drawText(_leftPadding + 4, 0, q->width(), q->height(), Qt::AlignVCenter, "!");
    // 时间进度条绘制
    QPainterPath clipPath;
    clipPath.addRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    painter->setClipPath(clipPath);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0xC4, 0xAD, 0x59));
    painter->drawRoundedRect(QRectF(foregroundRect.x(), foregroundRect.bottom() - _timePercentHeight, foregroundRect.width() * _pTimePercent / 100.0, _timePercentHeight + 1), 2, 2);
    painter->restore();
    // 文字颜色
    painter->setPen(Qt::black);
}

void ElaMessageBarPrivate::_drawInformation(QPainter* painter)
{
    Q_Q(ElaMessageBar);
    painter->save();
    painter->setBrush(QColor(0xF4, 0xF4, 0xF4));
    QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth, q->width() - 2 * _shadowBorderWidth, q->height() - 2 * _shadowBorderWidth);
    painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    // 图标绘制
    painter->setPen(Qt::white);
    QPainterPath textPath;
    textPath.addEllipse(QPoint(_leftPadding + 6, q->height() / 2), 9, 9);
    painter->setClipPath(textPath);
    painter->fillPath(textPath, QColor(0x00, 0x66, 0xB4));
    painter->drawText(_leftPadding + 4, 0, q->width(), q->height(), Qt::AlignVCenter, "i");
    // 时间进度条绘制
    QPainterPath clipPath;
    clipPath.addRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    painter->setClipPath(clipPath);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0x00, 0x66, 0xB4));
    painter->drawRoundedRect(QRectF(foregroundRect.x(), foregroundRect.bottom() - _timePercentHeight, foregroundRect.width() * _pTimePercent / 100.0, _timePercentHeight + 1), 2, 2);
    painter->restore();
    // 文字颜色
    painter->setPen(Qt::black);
}

void ElaMessageBarPrivate::_drawError(QPainter* painter)
{
    Q_Q(ElaMessageBar);
    painter->save();
    painter->setBrush(QColor(0xFD, 0xE7, 0xE9));
    QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth, q->width() - 2 * _shadowBorderWidth, q->height() - 2 * _shadowBorderWidth);
    painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    // 图标绘制
    painter->setPen(Qt::white);
    QPainterPath textPath;
    textPath.addEllipse(QPoint(_leftPadding + 6, q->height() / 2), 9, 9);
    painter->setClipPath(textPath);
    painter->fillPath(textPath, QColor(0xBA, 0x2D, 0x20));
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(13);
    painter->setFont(iconFont);
    painter->drawText(_leftPadding + 1, 0, q->width(), q->height(), Qt::AlignVCenter, QChar(ElaIconType::Xmark));
    // 时间进度条绘制
    QPainterPath clipPath;
    clipPath.addRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    painter->setClipPath(clipPath);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0xCC, 0x5C, 0x65));
    painter->drawRoundedRect(QRectF(foregroundRect.x(), foregroundRect.bottom() - _timePercentHeight, foregroundRect.width() * _pTimePercent / 100.0, _timePercentHeight + 1), 2, 2);
    painter->restore();
    // 文字颜色
    painter->setPen(Qt::black);
}
