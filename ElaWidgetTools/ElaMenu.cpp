#include "ElaMenu.h"

#include "ElaColorAnimation.h"
#include "ElaMenuStyle.h"
#include "ElaTheme.h"
#include "private/ElaMenuPrivate.h"

#include <QCursor>
#include <QGuiApplication>
#include <QMenuBar>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QScreen>

ElaMenu::ElaMenu(QWidget* parent)
    : QMenu(parent), d_ptr(new ElaMenuPrivate())
{
    Q_D(ElaMenu);
    d->q_ptr = this;
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setObjectName("ElaMenu");
    setContentsMargins(eTheme->getShadowMargins());
    d->_menuStyle = new ElaMenuStyle(style());
    d->_menuStyle->setParent(this);
    setStyle(d->_menuStyle);
    d->_showAnimation = new QPropertyAnimation(d, "pAnimationImagePosY", d);
    QEasingCurve popupEasing(QEasingCurve::BezierSpline);
    popupEasing.addCubicBezierSegment(QPointF(0, 0), QPointF(0, 1), QPointF(1, 1));
    d->_showAnimation->setEasingCurve(popupEasing);
    d->_showAnimation->setDuration(250);
    d->_showAnimation->setEndValue(0.0);
    connect(d->_showAnimation, &QPropertyAnimation::finished, d, &ElaMenuPrivate::_finishShowAnimation);
    connect(d->_showAnimation, &QPropertyAnimation::valueChanged, this, [this]() {
        update();
    });
}

ElaMenu::ElaMenu(const QString& title, QWidget* parent)
    : ElaMenu(parent)
{
    setTitle(title);
}

ElaMenu::~ElaMenu() = default;

void ElaMenu::setMenuItemHeight(int menuItemHeight)
{
    Q_D(ElaMenu);
    d->_menuStyle->setMenuItemHeight(menuItemHeight);
}

int ElaMenu::getMenuItemHeight() const
{
    Q_D(const ElaMenu);
    return d->_menuStyle->getMenuItemHeight();
}

QAction* ElaMenu::addMenu(QMenu* menu)
{
    return QMenu::addMenu(menu);
}

ElaMenu* ElaMenu::addMenu(const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    QMenu::addAction(menu->menuAction());
    return menu;
}

ElaMenu* ElaMenu::addMenu(const QIcon& icon, const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    menu->setIcon(icon);
    QMenu::addAction(menu->menuAction());
    return menu;
}

ElaMenu* ElaMenu::addMenu(ElaIconType::IconName icon, const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    QMenu::addAction(menu->menuAction());
    menu->menuAction()->setProperty("ElaIconType", QChar(icon));
    return menu;
}

QAction* ElaMenu::addElaIconAction(ElaIconType::IconName icon, const QString& text)
{
    QAction* action = new QAction(text, this);
    action->setProperty("ElaIconType", QChar(icon));
    QMenu::addAction(action);
    return action;
}

QAction* ElaMenu::addElaIconAction(ElaIconType::IconName icon, const QString& text, const QKeySequence& shortcut)
{
    QAction* action = new QAction(text, this);
    action->setShortcut(shortcut);
    action->setProperty("ElaIconType", QChar(icon));
    QMenu::addAction(action);
    return action;
}

bool ElaMenu::isHasChildMenu() const
{
    QList<QAction*> actionList = this->actions();
    for (auto action: actionList)
    {
        if (action->isSeparator())
        {
            continue;
        }
        if (action->menu())
        {
            return true;
        }
    }
    return false;
}

bool ElaMenu::isHasIcon() const
{
    QList<QAction*> actionList = this->actions();
    for (auto action: actionList)
    {
        if (action->isSeparator())
        {
            continue;
        }
        QMenu* menu = action->menu();
        if (menu && (!menu->icon().isNull() || !menu->property("ElaIconType").toString().isEmpty()))
        {
            return true;
        }
        if (!action->icon().isNull() || !action->property("ElaIconType").toString().isEmpty())
        {
            return true;
        }
    }
    return false;
}

void ElaMenu::showEvent(QShowEvent* event)
{
    Q_EMIT menuShow();
    Q_D(ElaMenu);
    d->_finishShowAnimation();
    const QMargins margins = eTheme->getShadowMargins();
    const QPoint initialPos = pos();
    const QPoint cursorPos = QCursor::pos();
    QWidget* popupSource = d->_getPopupSource();
    auto* parentMenu = qobject_cast<QMenu*>(popupSource);
    auto* parentMenuBar = qobject_cast<QMenuBar*>(popupSource);

    QPoint screenRefPos = initialPos;
    if (parentMenu)
    {
        screenRefPos = parentMenu->geometry().center();
    }
    else if (popupSource)
    {
        screenRefPos = popupSource->mapToGlobal(popupSource->rect().center());
    }
    QScreen* screen = QGuiApplication::screenAt(screenRefPos);
    if (!screen)
    {
        screen = QGuiApplication::primaryScreen();
    }
    const QRect available = screen ? screen->availableGeometry().adjusted(4, 4, -4, -4) : QRect();

    QPoint position = initialPos - QPoint(margins.left(), margins.top());
    bool isSlideDown = true;
    int subMenuTriggerCenterY = 0;

    if (parentMenu)
    {
        const QRect parentBody = parentMenu->geometry().marginsRemoved(qobject_cast<ElaMenu*>(parentMenu) ? margins : QMargins());
        const QRect parentActionRect = parentMenu->actionGeometry(parentMenu->activeAction());
        const QPoint parentActionTopLeft = parentMenu->mapToGlobal(parentActionRect.topLeft());
        subMenuTriggerCenterY = parentActionTopLeft.y() + parentActionRect.height() / 2;
        const int right = parentBody.right() + 5 - margins.left();
        const int left = parentBody.left() - 4 - width() + margins.right();
        bool isLeft = false;
        if (available.isValid())
        {
            isLeft = parentMenu->isRightToLeft()
                ? (left + margins.left() >= available.left())
                : (right + width() - margins.right() > available.right() + 1);
        }
        position.setX(isLeft ? left : right);
        const QList<QAction*> actionList = actions();
        position.setY(parentActionTopLeft.y() - (actionList.isEmpty() ? margins.top() + 3 : actionGeometry(actionList.first()).top()));
    }
    else if (popupSource)
    {
        const QRect sourceRect(popupSource->mapToGlobal(QPoint(0, 0)), popupSource->size());
        const bool isAboveSource = initialPos.y() + height() <= sourceRect.top() + 2;
        if (isAboveSource)
        {
            // 原生弹出已翻转到上方时 面板底边对齐触发控件顶边
            position.setY(initialPos.y() + margins.bottom());
        }
        const int bodyCenterY = position.y() + margins.top() + (height() - margins.top() - margins.bottom()) / 2;
        isSlideDown = parentMenuBar ? !isAboveSource : bodyCenterY >= sourceRect.center().y();
    }
    else
    {
        const bool isCursorNearX = cursorPos.x() >= initialPos.x() - 48 && cursorPos.x() <= initialPos.x() + width() + 48;
        const bool isCursorNearY = cursorPos.y() >= initialPos.y() - 48 && cursorPos.y() <= initialPos.y() + height() + 48;
        if (isCursorNearX && isCursorNearY)
        {
            // 普通弹出靠近屏幕边缘被原生逻辑翻转时 改按面板对应边缘对齐触发点
            if (initialPos.x() + width() / 2 < cursorPos.x() && qAbs((initialPos.x() + width()) - cursorPos.x()) <= 40)
            {
                position.setX(initialPos.x() + margins.right());
            }
            if (initialPos.y() + height() / 2 < cursorPos.y() && qAbs((initialPos.y() + height()) - cursorPos.y()) <= 40)
            {
                position.setY(initialPos.y() + margins.bottom());
                isSlideDown = false;
            }
        }
    }

    if (available.isValid())
    {
        // 仅约束菜单可见面板在可用屏幕内 允许外圈透明阴影自然延伸贴边
        const int minX = available.left() - margins.left();
        const int maxX = qMax(minX, available.right() - width() + margins.right() + 1);
        const int minY = available.top() - margins.top();
        const int maxY = qMax(minY, available.bottom() - height() + margins.bottom() + 1);
        position.setX(qBound(minX, position.x(), maxX));
        position.setY(qBound(minY, position.y(), maxY));
    }
    move(position);
    updateGeometry();

    const QRect foregroundRect = rect().marginsRemoved(margins);
    const int bodyHeight = foregroundRect.height();
    if (parentMenu)
    {
        const int slideDownThreshold = y() + margins.top() + qMax(d->_menuStyle->getMenuItemHeight() + 3, bodyHeight / 2);
        isSlideDown = subMenuTriggerCenterY <= slideDownThreshold;
    }
    else if (!popupSource && isSlideDown)
    {
        const int panelLeft = x() + margins.left();
        const int panelRight = x() + width() - margins.right();
        const int panelTop = y() + margins.top();
        const int panelBottom = y() + height() - margins.bottom();
        const int slideUpThreshold = panelTop + qMax(d->_menuStyle->getMenuItemHeight() + 12, (bodyHeight * 2) / 3);
        if (cursorPos.x() >= panelLeft - 48 && cursorPos.x() <= panelRight + 48
            && cursorPos.y() > slideUpThreshold && cursorPos.y() <= panelBottom + 8)
        {
            isSlideDown = false;
        }
    }

    d->_startShowAnimation(isSlideDown, bodyHeight);
    QMenu::showEvent(event);
}

void ElaMenu::hideEvent(QHideEvent* event)
{
    Q_D(ElaMenu);
    d->_finishShowAnimation();
    QMenu::hideEvent(event);
}

void ElaMenu::paintEvent(QPaintEvent* event)
{
    Q_D(ElaMenu);
    if (!d->_animationPix.isNull())
    {
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        const QMargins margins = eTheme->getShadowMargins();
        const QRect foregroundRect = rect().marginsRemoved(margins);
        if (foregroundRect.isEmpty())
        {
            return;
        }
        const qreal dpr = qMax(1.0, devicePixelRatioF());
        const qreal bodyHeight = static_cast<qreal>(foregroundRect.height());
        const qreal alignedOffsetY = qBound(-bodyHeight + 1.0, qRound(d->_pAnimationImagePosY * dpr) / dpr, bodyHeight - 1.0);
        const QRectF currentBodyRect = QRectF(foregroundRect).adjusted(0.0, qMax(0.0, alignedOffsetY), 0.0, qMin(0.0, alignedOffsetY));

        // 外圈柔和阴影随当前展开面板高度同步伸缩并平滑过渡浓度
        const QRect currentShadowRect = currentBodyRect.toAlignedRect().marginsAdded(margins);
        const qreal shadowRatio = currentBodyRect.height() / bodyHeight;
        eTheme->drawEffectShadow(&painter, currentShadowRect, margins, 6, shadowRatio);

        const ElaThemeType::ThemeMode themeMode = eTheme->getThemeMode();
        painter.setPen(Qt::NoPen);
        painter.setBrush(ElaColorAnimation::getColor(this, ElaThemeColor(themeMode, PopupBase)));
        painter.drawRoundedRect(currentBodyRect, 6, 6);

        // 严格在可见圆角面板内侧裁剪绘制截图 锚定触发边不越界进入阴影区
        if (currentBodyRect.width() > 2.0 && currentBodyRect.height() > 2.0)
        {
            painter.save();
            QPainterPath clipPath;
            clipPath.addRoundedRect(currentBodyRect.adjusted(1.0, 1.0, -1.0, -1.0), 5, 5);
            painter.setClipPath(clipPath);
            painter.drawPixmap(QPointF(0.0, alignedOffsetY), d->_animationPix);
            painter.restore();
        }

        painter.setPen(ElaThemeColor(themeMode, BasicBorder));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(currentBodyRect, 6, 6);
    }
    else
    {
        QMenu::paintEvent(event);
    }
}
