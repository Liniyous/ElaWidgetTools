#include "ElaFlowLayout.h"

#include <QPropertyAnimation>
#include <QWidget>

#include "ElaEventBus.h"
ElaFlowLayout::ElaFlowLayout(QWidget* parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), _hSpacing(hSpacing), _vSpacing(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
    _navigationAnimationEvent = new ElaEvent("NavigationAnimationStateChanged", this);
    ElaEventBus::getInstance()->registerEvent(_navigationAnimationEvent);
    connect(_navigationAnimationEvent, &ElaEvent::triggered, this, &ElaFlowLayout::onNavgationAnimationStateChanged);
    _lastHeightMap.insert(this, 0);
}

ElaFlowLayout::ElaFlowLayout(int margin, int hSpacing, int vSpacing)
    : _hSpacing(hSpacing), _vSpacing(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
    _navigationAnimationEvent = new ElaEvent("NavigationAnimationStateChanged", this);
    ElaEventBus::getInstance()->registerEvent(_navigationAnimationEvent);
    connect(_navigationAnimationEvent, &ElaEvent::triggered, this, &ElaFlowLayout::onNavgationAnimationStateChanged);
    _lastHeightMap.insert(this, 0);
}

ElaFlowLayout::~ElaFlowLayout()
{
    QLayoutItem* item;
    while ((item = this->takeAt(0)))
    {
        delete item;
    }
}

void ElaFlowLayout::addItem(QLayoutItem* item)
{
    _itemList.append(item);
}
int ElaFlowLayout::horizontalSpacing() const
{
    if (_hSpacing >= 0)
    {
        return _hSpacing;
    }
    else
    {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int ElaFlowLayout::verticalSpacing() const
{
    if (_vSpacing >= 0)
    {
        return _vSpacing;
    }
    else
    {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}
int ElaFlowLayout::count() const
{
    return _itemList.size();
}

QLayoutItem* ElaFlowLayout::itemAt(int index) const
{
    return _itemList.value(index);
}

QLayoutItem* ElaFlowLayout::takeAt(int index)
{
    if (index >= 0 && index < _itemList.size())
        return _itemList.takeAt(index);
    return nullptr;
}

void ElaFlowLayout::setIsAnimation(bool isAnimation)
{
    this->_isAnimation = isAnimation;
}

void ElaFlowLayout::onNavgationAnimationStateChanged(QMap<QString, QVariant> data)
{
    if (data.contains("NavigationAnimationState"))
    {
        this->_isNavigationAnimation = data.value("NavigationAnimationState").toBool();
    }
}

Qt::Orientations ElaFlowLayout::expandingDirections() const
{
    return {};
}

bool ElaFlowLayout::hasHeightForWidth() const
{
    return true;
}

int ElaFlowLayout::heightForWidth(int width) const
{
    if (this->_isNavigationAnimation)
    {
        return _lastHeightMap[const_cast<ElaFlowLayout*>(this)];
    }
    else
    {
        int height = doLayout(QRect(0, 0, width, 0), true);
        _lastHeightMap[const_cast<ElaFlowLayout*>(this)] = height;
        return height;
    }
}

void ElaFlowLayout::setGeometry(const QRect& rect)
{
    if (this->_isNavigationAnimation && !this->_isAnimation)
    {
        return;
    }
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize ElaFlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize ElaFlowLayout::minimumSize() const
{
    QSize size;
    for (const QLayoutItem* item : std::as_const(_itemList))
    {
        size = size.expandedTo(item->minimumSize());
    }

    const QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}

int ElaFlowLayout::doLayout(const QRect& rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    for (QLayoutItem* item : std::as_const(_itemList))
    {
        const QWidget* wid = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1)
        {
            spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        }
        int spaceY = verticalSpacing();
        if (spaceY == -1)
        {
            spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        }

        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0)
        {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }
        if (!_lastGeometryMap.contains(item))
        {
            _lastGeometryMap.insert(item, QPoint(x, y));
        }
        if (!testOnly)
        {
            if ((item->geometry().x() == 0 && item->geometry().y() == 0))
            {
                item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
            }
            else if (item->geometry().x() != x || item->geometry().y() != y)
            {
                if (_isAnimation)
                {
                    // 阻止多重动画
                    if (_lastGeometryMap[item] == QPoint(x, y))
                    {
                        lineHeight = qMax(lineHeight, item->sizeHint().height());
                        return y + lineHeight - rect.y() + bottom;
                    }
                    QPropertyAnimation* geometryAnimation = new QPropertyAnimation(item->widget(), "geometry");
                    geometryAnimation->setStartValue(item->widget()->geometry());
                    geometryAnimation->setEndValue(QRect(QPoint(x, y), item->sizeHint()));
                    geometryAnimation->setDuration(225);
                    geometryAnimation->setEasingCurve(QEasingCurve::InOutSine);
                    geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
                    _lastGeometryMap[item] = QPoint(x, y);
                }
                else
                {
                    item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
                }
            }
        }
        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}

int ElaFlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject* parent = this->parent();
    if (!parent)
    {
        return -1;
    }
    else if (parent->isWidgetType())
    {
        QWidget* pw = static_cast<QWidget*>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    }
    else
    {
        return static_cast<QLayout*>(parent)->spacing();
    }
}
