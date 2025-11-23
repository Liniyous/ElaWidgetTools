#include "ElaRoller.h"
#include "ElaRollerPrivate.h"
#include "ElaTheme.h"
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QWheelEvent>
#include <QtMath>
#include <utility>
Q_PROPERTY_CREATE_Q_CPP(ElaRoller, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaRoller, bool, IsContainer)
Q_PROPERTY_CREATE_Q_CPP(ElaRoller, bool, IsEnableLoop)
ElaRoller::ElaRoller(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaRollerPrivate())
{
    Q_D(ElaRoller);
    d->q_ptr = this;
    d->_pScrollOffset = 0;
    d->_pItemHeight = 35;
    d->_pBorderRadius = 3;
    d->_pMaxVisibleItems = 5;
    d->_pCurrentIndex = 0;
    d->_pIsEnableLoop = true;
    d->_pIsContainer = false;
    setFixedWidth(90);
    setFixedSize(90, 175);
    setMouseTracking(true);
    setObjectName("ElaRoller");
    setStyleSheet("#ElaRoller{background-color:transparent;}");
    QFont font = this->font();
    font.setPixelSize(16);
    setFont(font);

    d->_scrollAnimation = new QPropertyAnimation(d, "pScrollOffset");
    connect(d->_scrollAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
        update();
    });
    connect(d->_scrollAnimation, &QPropertyAnimation::finished, this, [=]() {
        while (d->_pScrollOffset < -d->_pItemList.size() * d->_pItemHeight)
        {
            d->_pScrollOffset += d->_pItemList.size() * d->_pItemHeight;
        }
        while (d->_pScrollOffset >= d->_pItemList.size() * d->_pItemHeight)
        {
            d->_pScrollOffset -= d->_pItemList.size() * d->_pItemHeight;
        }
        d->_targetScrollOffset = d->_pScrollOffset;
        int currentIndex = 0;
        if (d->_pScrollOffset >= 0)
        {
            currentIndex = d->_pScrollOffset / d->_pItemHeight;
        }
        else if (d->_pScrollOffset <= -d->_pItemHeight)
        {
            currentIndex = d->_pItemList.count() + d->_pScrollOffset / d->_pItemHeight;
        }
        if (currentIndex != d->_pCurrentIndex)
        {
            d->_pCurrentIndex = currentIndex;
            Q_EMIT pCurrentIndexChanged();
            Q_EMIT currentDataChanged(getCurrentData());
        }
        update();
    });

    d->_scrollAnimation->setDuration(300);
    d->_scrollAnimation->setEasingCurve(QEasingCurve::OutCubic);

    d->_pressSustainTimer = new QTimer(this);
    d->_pressSustainTimer->setInterval(300);
    connect(d->_pressSustainTimer, &QTimer::timeout, this, [=]() {
        d->_repeatScrollTimer->start();
    });

    d->_repeatScrollTimer = new QTimer(this);
    d->_repeatScrollTimer->setInterval(50);
    connect(d->_repeatScrollTimer, &QTimer::timeout, this, [=]() {
        if (d->_isUpArrowPress)
        {
            d->_scroll(120);
        }
        if (d->_isDownArrowPress)
        {
            d->_scroll(-120);
        }
    });

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaRoller::~ElaRoller()
{
}

void ElaRoller::setCurrentData(const QString& data)
{
    Q_D(ElaRoller);
    if (d->_pItemList.contains(data))
    {
        setCurrentIndex(d->_pItemList.indexOf(data));
    }
}

QString ElaRoller::getCurrentData() const
{
    Q_D(const ElaRoller);
    if (d->_pCurrentIndex >= d->_pItemList.count())
    {
        return {};
    }
    return d->_pItemList[d->_pCurrentIndex];
}

void ElaRoller::setItemList(QStringList itemList)
{
    Q_D(ElaRoller);
    d->_pItemList = std::move(itemList);
    update();
    Q_EMIT pItemListChanged();
}

QStringList ElaRoller::getItemList() const
{
    Q_D(const ElaRoller);
    return d->_pItemList;
}

void ElaRoller::setItemHeight(int itemHeight)
{
    Q_D(ElaRoller);
    d->_pItemHeight = itemHeight;
    setFixedHeight(d->_pMaxVisibleItems * d->_pItemHeight);
    update();
    Q_EMIT pItemHeightChanged();
}

int ElaRoller::getItemHeight() const
{
    Q_D(const ElaRoller);
    return d->_pItemHeight;
}

void ElaRoller::setMaxVisibleItems(int maxVisibleItems)
{
    Q_D(ElaRoller);
    d->_pMaxVisibleItems = maxVisibleItems;
    setFixedHeight(d->_pMaxVisibleItems * d->_pItemHeight);
    update();
    Q_EMIT pMaxVisibleItemsChanged();
}

int ElaRoller::getMaxVisibleItems() const
{
    Q_D(const ElaRoller);
    return d->_pMaxVisibleItems;
}

void ElaRoller::setCurrentIndex(int currentIndex)
{
    Q_D(ElaRoller);
    if (currentIndex >= d->_pItemList.count())
    {
        return;
    }
    d->_pCurrentIndex = currentIndex;
    d->_pScrollOffset = d->_pItemHeight * currentIndex;
    d->_targetScrollOffset = d->_pScrollOffset;
    update();
}

int ElaRoller::getCurrentIndex() const
{
    Q_D(const ElaRoller);
    return d->_pCurrentIndex;
}

void ElaRoller::wheelEvent(QWheelEvent* event)
{
    Q_D(ElaRoller);
    d->_scroll(event->angleDelta().y());
    event->accept();
}

void ElaRoller::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaRoller);
    if (d->_pIsContainer && d->_pMaxVisibleItems >= 5)
    {
        auto currentPos = event->pos();
        if (d->_upArrowRect.contains(currentPos))
        {
            d->_isUpArrowPress = true;
            d->_scroll(120);
            d->_pressSustainTimer->start();
        }
        else if (d->_downArrowRect.contains(currentPos))
        {
            d->_isDownArrowPress = true;
            d->_scroll(-120);
            d->_pressSustainTimer->start();
        }
    }
    QWidget::mousePressEvent(event);
}

void ElaRoller::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaRoller);
    int centerIndex = d->_pMaxVisibleItems / 2;
    int clickIndex = event->pos().y() / d->_pItemHeight;
    int jumpCount = abs(d->_pMaxVisibleItems / 2 - clickIndex);
    if (d->_isUpArrowPress || d->_isDownArrowPress)
    {
        d->_pressSustainTimer->stop();
        d->_repeatScrollTimer->stop();
        d->_isUpArrowPress = false;
        d->_isDownArrowPress = false;
        update();
    }
    else
    {
        if (clickIndex > centerIndex)
        {
            for (int i = 0; i < jumpCount; i++)
            {
                d->_scroll(-120);
            }
        }
        else if (clickIndex < centerIndex)
        {
            for (int i = 0; i < jumpCount; i++)
            {
                d->_scroll(120);
            }
        }
    }
    update();
}

void ElaRoller::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaRoller);
    d->_mousePoint = event->pos();
    update();
    QWidget::mouseMoveEvent(event);
}

void ElaRoller::leaveEvent(QEvent* event)
{
    Q_D(ElaRoller);
    d->_mousePoint = QPoint();
    update();
    QWidget::leaveEvent(event);
}

void ElaRoller::paintEvent(QPaintEvent* event)
{
    Q_D(ElaRoller);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 背景绘制
    QRect foregroundRect(d->_penBorderWidth, d->_penBorderWidth, width() - 2 * d->_penBorderWidth, height() - 2 * d->_penBorderWidth - 1);
    if (!d->_pIsContainer)
    {
        painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), d->_penBorderWidth));
        painter.setBrush(isEnabled() ? ElaThemeColor(d->_themeMode, BasicBase) : ElaThemeColor(d->_themeMode, BasicDisable));
        painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    }
    painter.setClipRect(foregroundRect);
    // 中心指示区域绘制
    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaThemeColor(d->_themeMode, PrimaryNormal));
    bool isEven = d->_pMaxVisibleItems % 2 == 0;
    if (isEven)
    {
        painter.drawRoundedRect(QRect(d->_indicatorMargin, d->_pItemHeight * d->_pMaxVisibleItems / 2 + d->_indicatorMargin, width() - 2 * d->_indicatorMargin, d->_pItemHeight - 2 * d->_indicatorMargin), d->_pBorderRadius, d->_pBorderRadius);
    }
    else
    {
        painter.drawRoundedRect(QRect(d->_indicatorMargin, (height() - d->_pItemHeight) / 2 + d->_indicatorMargin, width() - 2 * d->_indicatorMargin, d->_pItemHeight - 2 * d->_indicatorMargin), d->_pBorderRadius, d->_pBorderRadius);
    }

    int yStart = -d->_pItemHeight;
    int yEnd = height();
    int centerYStart = isEven ? d->_pItemHeight * d->_pMaxVisibleItems / 2 : height() / 2 - d->_pItemHeight;
    int centerYEnd = isEven ? d->_pItemHeight * d->_pMaxVisibleItems / 2 + d->_pItemHeight / 2 : height() / 2;
    int startIndexOffset = d->_pMaxVisibleItems / 2;
    for (int i = 0; i < d->_pItemList.size(); i++)
    {
        qreal y = (i + startIndexOffset) * d->_pItemHeight - d->_pScrollOffset;
        // 范围控制
        if (d->_pIsEnableLoop)
        {
            while (y <= yStart)
            {
                y += d->_pItemList.size() * d->_pItemHeight;
            }
            while (y >= yEnd)
            {
                y -= d->_pItemList.size() * d->_pItemHeight;
            }
        }
        // 可见区域绘制
        if (y >= yStart && y <= yEnd)
        {
            painter.save();
            painter.translate(0, y);
            // 文字绘制
            if (y >= centerYStart && y <= centerYEnd)
            {
                painter.setPen(ElaThemeColor(d->_themeMode, BasicTextInvert));
            }
            else
            {
                // 覆盖效果绘制
                if (!d->_mousePoint.isNull() && d->_scrollAnimation->state() == QPropertyAnimation::State::Stopped)
                {
                    QRectF itemRect = QRectF(0, y, width(), d->_pItemHeight);
                    if (itemRect.contains(d->_mousePoint))
                    {
                        painter.setBrush(ElaThemeColor(d->_themeMode, BasicHover));
                        painter.drawRoundedRect(QRect(d->_indicatorMargin, d->_indicatorMargin, width() - 2 * d->_indicatorMargin, d->_pItemHeight - 2 * d->_indicatorMargin), d->_pBorderRadius, d->_pBorderRadius);
                    }
                }
                painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
            }
            painter.drawText(QRect(0, 0, width(), d->_pItemHeight), Qt::AlignCenter | Qt::TextSingleLine, d->_pItemList[i]);
            painter.restore();
        }
    }
    // Container模式指示器绘制
    if (d->_pIsContainer && d->_pMaxVisibleItems >= 5 && underMouse())
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(ElaThemeColor(d->_themeMode, PopupBase));
        // 覆盖文字
        d->_upArrowRect = QRect(foregroundRect.x(), foregroundRect.y(), foregroundRect.width(), d->_pItemHeight);
        d->_downArrowRect = QRect(foregroundRect.x(), foregroundRect.bottom() - d->_pItemHeight + 2, foregroundRect.width(), d->_pItemHeight);
        painter.drawRect(d->_upArrowRect);
        painter.drawRect(d->_downArrowRect);
        painter.setBrush(ElaThemeColor(d->_themeMode, BasicText));
        // 上箭头
        int upArrowWidth = d->_isUpArrowPress ? 8 : 10; // 边长
        qreal upAdjacentEdge = upArrowWidth * qCos(30 * M_PI / 180.0);
        QPainterPath upArrowPath;
        upArrowPath.moveTo(d->_upArrowRect.center().x(), d->_upArrowRect.center().y() - upAdjacentEdge / 2);
        upArrowPath.lineTo(d->_upArrowRect.center().x() + upArrowWidth / 2, d->_upArrowRect.center().y() + upAdjacentEdge / 2);
        upArrowPath.lineTo(d->_upArrowRect.center().x() - upArrowWidth / 2, d->_upArrowRect.center().y() + upAdjacentEdge / 2);
        upArrowPath.closeSubpath();
        painter.drawPath(upArrowPath);
        // 下箭头
        int downArrowWidth = d->_isDownArrowPress ? 8 : 10;
        qreal downAdjacentEdge = downArrowWidth * qCos(30 * M_PI / 180.0);
        QPainterPath downArrowPath;
        downArrowPath.moveTo(d->_downArrowRect.center().x(), d->_downArrowRect.center().y() + downAdjacentEdge / 2);
        downArrowPath.lineTo(d->_downArrowRect.center().x() + downArrowWidth / 2, d->_downArrowRect.center().y() - downAdjacentEdge / 2);
        downArrowPath.lineTo(d->_downArrowRect.center().x() - downArrowWidth / 2, d->_downArrowRect.center().y() - downAdjacentEdge / 2);
        downArrowPath.closeSubpath();
        painter.drawPath(downArrowPath);
    }
    painter.restore();
}
