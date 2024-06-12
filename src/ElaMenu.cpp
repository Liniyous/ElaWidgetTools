#include "ElaMenu.h"

#include <QApplication>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QScreen>
#include <QUuid>
#include <QVBoxLayout>

#include "DeveloperComponents/ElaMenuDelegate.h"
#include "DeveloperComponents/ElaMenuModel.h"
#include "ElaApplication.h"
#include "ElaEventBus.h"
#include "ElaListView.h"
#include "private/ElaMenuPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaMenu, int, BorderRadius)
ElaMenu::ElaMenu(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaMenuPrivate())
{
    Q_D(ElaMenu);
    d->q_ptr = this;
    d->_menuCheckSumKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    setObjectName("ElaMenu");
    setFixedWidth(200);
    d->_pBorderRadius = 6;
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::NoDropShadowWindowHint | Qt::FramelessWindowHint);
    setMouseTracking(true);
    setAttribute(Qt::WA_TranslucentBackground);
    d->_windowLinearGradient = new QLinearGradient(0, 0, width(), height());
    d->_windowLinearGradient->setColorAt(0, QColor(0xF2, 0xE7, 0xF5));
    d->_windowLinearGradient->setColorAt(1, QColor(0xED, 0xF3, 0xF9));
    d->_centerView = new ElaListView(this);
    d->_centerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_centerDelegate = new ElaMenuDelegate(this);
    d->_centerView->setItemDelegate(d->_centerDelegate);
    connect(d->_centerView, &ElaListView::clicked, d, &ElaMenuPrivate::onMenuClicked);
    d->_centerModel = new ElaMenuModel(this);
    d->_centerView->setModel(d->_centerModel);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(d->_shadowBorderWidth, d->_shadowBorderWidth + 5, d->_shadowBorderWidth, d->_shadowBorderWidth + 5);
    mainLayout->addWidget(d->_centerView);
    hide();

    // 事件总线
    d->_menuEvent = new ElaEvent("ElaMenuEvent", this);
    ElaEventBus::getInstance()->registerEvent(d->_menuEvent);
    connect(d->_menuEvent, &ElaEvent::triggered, d, &ElaMenuPrivate::onElaMenuEvent);

    connect(ElaApplication::getInstance(), &ElaApplication::themeModeChanged, d, &ElaMenuPrivate::onThemeChanged);
}

ElaMenu::~ElaMenu()
{
}

void ElaMenu::setMenuItemHeight(int height)
{
    Q_D(ElaMenu);
    d->_menuItemHeight = height;
    d->_centerDelegate->setMenuItemHeight(height);
}

void ElaMenu::popup(const QPoint& pos, QAction* at)
{
    // 隐藏子菜单
    Q_D(ElaMenu);
    d->_isCloseAnimationFinished = true;
    d->_centerView->clearSelection();
    if (d->_isTopMostMenu)
    {
        QMap<QString, QVariant> postData;
        postData.insert("ElaMenuCheckSumKey", d->_menuCheckSumKey);
        postData.insert("HideAllMenu", "");
        ElaEventBus::getInstance()->post("ElaMenuEvent", postData);
    }
    // 计算坐标点合理性
    QRect screenRect = QApplication::screenAt(QCursor::pos())->geometry();
    QPoint finalPos = pos;
    // 判断popup方位
    if (finalPos.x() + width() > screenRect.width())
    {
        finalPos.setX(pos.x() - width());
    }
    if (finalPos.y() + height() > screenRect.height())
    {
        finalPos.setY(pos.y() - height());
    }
    this->move(finalPos);
    this->show();
    int minimumHeight = d->_menuItemHeight * d->_actionList.count() + 2 * d->_shadowBorderWidth + 10;
    d->_windowLinearGradient->setFinalStop(QPointF(width(), minimumHeight));
    QPropertyAnimation* menuSizeAnimation = new QPropertyAnimation(this, "minimumHeight");
    menuSizeAnimation->setEasingCurve(QEasingCurve::OutCubic);
    menuSizeAnimation->setStartValue(minimumHeight / 2);
    menuSizeAnimation->setEndValue(minimumHeight);
    menuSizeAnimation->setDuration(400);
    menuSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    QPropertyAnimation* menuSizeAnimation1 = new QPropertyAnimation(this, "maximumHeight");
    menuSizeAnimation1->setEasingCurve(QEasingCurve::OutCubic);
    menuSizeAnimation1->setStartValue(minimumHeight / 2);
    menuSizeAnimation1->setEndValue(minimumHeight);
    menuSizeAnimation1->setDuration(400);
    menuSizeAnimation1->start(QAbstractAnimation::DeleteWhenStopped);

    QPropertyAnimation* viewPosAnimation = new QPropertyAnimation(d->_centerView, "pos");
    QPoint centerViewpos = d->_centerView->pos();
    viewPosAnimation->setEasingCurve(QEasingCurve::OutCubic);
    viewPosAnimation->setStartValue(QPoint(centerViewpos.x(), centerViewpos.y() - d->_centerView->height() / 2));
    viewPosAnimation->setEndValue(centerViewpos);
    viewPosAnimation->setDuration(400);
    viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "windowOpacity");
    opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    opacityAnimation->setStartValue(0);
    opacityAnimation->setEndValue(1);
    opacityAnimation->setDuration(200);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

ElaMenu* ElaMenu::addMenu(const QString& text)
{
    Q_D(ElaMenu);
    ElaMenu* menu = new ElaMenu(this);
    menu->setMenuItemHeight(d->_menuItemHeight);
    menu->d_ptr->_parentMenu = this;
    d->_childMenus.append(menu);
    menu->d_ptr->_menuCheckSumKey = d_ptr->_menuCheckSumKey;
    menu->d_ptr->_isTopMostMenu = false;
    menu->d_ptr->_menuDepth = d->_menuDepth + 1;
    QAction* action = new QAction(text, this);
    action->setData(QVariant::fromValue(menu));
    d->_actionList.append(action);
    d->_centerModel->appendAction(action);
    return menu;
}

ElaMenu* ElaMenu::addMenu(ElaIconType icon, const QString& text)
{
    Q_D(ElaMenu);
    ElaMenu* menu = new ElaMenu(this);
    menu->setMenuItemHeight(d->_menuItemHeight);
    menu->d_ptr->_parentMenu = this;
    d->_childMenus.append(menu);
    menu->d_ptr->_menuCheckSumKey = d_ptr->_menuCheckSumKey;
    menu->d_ptr->_menuDepth = d->_menuDepth + 1;
    menu->d_ptr->_isTopMostMenu = false;
    QAction* action = new QAction(text, this);
    action->setProperty("ElaIconType", QChar((unsigned short)icon));
    action->setData(QVariant::fromValue(menu));
    d->_actionList.append(action);
    d->_centerModel->appendAction(action);
    return menu;
}

QAction* ElaMenu::addMenu(ElaMenu* menu)
{
    Q_D(ElaMenu);
    if (!menu)
    {
        return nullptr;
    }
    menu->setMenuItemHeight(d->_menuItemHeight);
    menu->d_ptr->_isTopMostMenu = false;
    menu->d_ptr->_menuDepth = d->_menuDepth + 1;
    menu->d_ptr->_parentMenu = this;
    d->_childMenus.append(menu);
    menu->d_ptr->_menuCheckSumKey = d_ptr->_menuCheckSumKey;
    QAction* action = new QAction(this);
    action->setData(QVariant::fromValue(menu));
    d->_actionList.append(action);
    d->_centerModel->appendAction(action);
    return action;
}

QAction* ElaMenu::addAction(const QString& text)
{
    Q_D(ElaMenu);
    QAction* action = new QAction(text, this);
    d->_actionList.append(action);
    d->_centerModel->appendAction(action);
    return action;
}

QAction* ElaMenu::addAction(ElaIconType icon, const QString& text)
{
    Q_D(ElaMenu);
    QAction* action = new QAction(text, this);
    action->setProperty("ElaIconType", QChar((unsigned short)icon));
    d->_actionList.append(action);
    d->_centerModel->appendAction(action);
    return action;
}

QAction* ElaMenu::addAction(const QString& text, const QKeySequence& shortcut)
{
    Q_D(ElaMenu);
    QAction* action = new QAction(text, this);
    action->setShortcut(shortcut);
    d->_actionList.append(action);
    d->_centerModel->appendAction(action);
    return action;
}

QAction* ElaMenu::addAction(ElaIconType icon, const QString& text, const QKeySequence& shortcut)
{
    Q_D(ElaMenu);
    QAction* action = new QAction(text, this);
    action->setProperty("ElaIconType", QChar((unsigned short)icon));
    action->setShortcut(shortcut);
    d->_actionList.append(action);
    d->_centerModel->appendAction(action);
    return action;
}

bool ElaMenu::event(QEvent* event)
{
    Q_D(ElaMenu);
    switch (event->type())
    {
    case QEvent::Show:
    {
        if (d->_isTopMostMenu)
        {
            activateWindow();
        }
        break;
    }
    case QEvent::WindowDeactivate:
    {
        // 判断所有菜单是否在鼠标下
        ElaMenu* parentMenu = this;
        while (!parentMenu->d_ptr->_isTopMostMenu)
        {
            parentMenu = parentMenu->d_ptr->_parentMenu;
        }
        if (!d->_containsCursorToAllMenu(parentMenu, parentMenu->d_ptr->_childMenus))
        {
            QMap<QString, QVariant> postData;
            postData.insert("ElaMenuCheckSumKey", d->_menuCheckSumKey);
            postData.insert("CloseAllMenu", "");
            ElaEventBus::getInstance()->post("ElaMenuEvent", postData);
        }
        break;
    }
    case QEvent::ToolTip:
    {
        QModelIndex modelIndex = d->_centerView->indexAt(mapFromGlobal(QCursor::pos()));
        if (d->_actionList.at(modelIndex.row())->data().value<ElaMenu*>())
        {
            d->onMenuClicked(modelIndex);
        }

        break;
    }
    case QEvent::MouseButtonPress:
    {
        QMap<QString, QVariant> postData;
        postData.insert("ElaMenuCheckSumKey", d->_menuCheckSumKey);
        postData.insert("CloseAllMenu", "");
        ElaEventBus::getInstance()->post("ElaMenuEvent", postData);
        break;
    }
    default:
    {
        break;
    }
    }
    return QWidget::event(event);
}

void ElaMenu::paintEvent(QPaintEvent* event)
{
    Q_D(ElaMenu);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

    // 高性能阴影
    painter.save();
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QColor color = QColor(0x60, 0x5F, 0x61);
    for (int i = 0; i < d->_shadowBorderWidth; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(d->_shadowBorderWidth - i, d->_shadowBorderWidth - i, this->width() - (d->_shadowBorderWidth - i) * 2, this->height() - (d->_shadowBorderWidth - i) * 2, d->_pBorderRadius + i, d->_pBorderRadius + i);
        int alpha = 5 * (d->_shadowBorderWidth - i + 1);
        color.setAlpha(alpha > 255 ? 255 : alpha);
        painter.setPen(color);
        painter.drawPath(path);
    }
    painter.restore();

    QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * d->_shadowBorderWidth, height() - 2 * d->_shadowBorderWidth);
    painter.setPen(Qt::NoPen);
    painter.setOpacity(0.98);
    painter.setBrush(*d->_windowLinearGradient);
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.restore();
}

void ElaMenu::closeEvent(QCloseEvent* event)
{
    Q_D(ElaMenu);
    event->ignore();
    if (!d->_isCloseAnimationFinished)
    {
        return;
    }
    d->_isCloseAnimationFinished = false;
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "windowOpacity");
    connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() {
        d->_isCloseAnimationFinished = true;
        this->hide(); });
    opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    opacityAnimation->setStartValue(1);
    opacityAnimation->setEndValue(0);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
