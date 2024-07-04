#include "ElaMenu.h"

#include <QApplication>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QVBoxLayout>

#include "DeveloperComponents/ElaMenuStyle.h"
#include "ElaApplication.h"
#include "private/ElaMenuPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaMenu, int, BorderRadius)
ElaMenu::ElaMenu(QWidget* parent)
    : QMenu(parent), d_ptr(new ElaMenuPrivate())
{
    Q_D(ElaMenu);
    d->q_ptr = this;
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setObjectName("ElaMenu");
    setStyle(new ElaMenuStyle(style()));
}

ElaMenu::ElaMenu(const QString& title, QWidget* parent)
    : QMenu(title, parent), d_ptr(new ElaMenuPrivate())
{
    Q_D(ElaMenu);
    d->q_ptr = this;
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setObjectName("ElaMenu");
    setStyle(new ElaMenuStyle(style()));
}

ElaMenu::~ElaMenu()
{
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

ElaMenu* ElaMenu::addMenu(ElaIconType icon, const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    QMenu::addAction(menu->menuAction());
    menu->menuAction()->setProperty("ElaIconType", QChar((unsigned short)icon));
    return menu;
}

QAction* ElaMenu::addElaIconAction(ElaIconType icon, const QString& text)
{
    QAction* action = new QAction(text, this);
    action->setProperty("ElaIconType", QChar((unsigned short)icon));
    QMenu::addAction(action);
    return action;
}

QAction* ElaMenu::addElaIconAction(ElaIconType icon, const QString& text, const QKeySequence& shortcut)
{
    QAction* action = new QAction(text, this);
    action->setShortcut(shortcut);
    action->setProperty("ElaIconType", QChar((unsigned short)icon));
    QMenu::addAction(action);
    return action;
}

bool ElaMenu::event(QEvent* event)
{
    Q_D(ElaMenu);
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);

        if (mouseEvent)
        {
            d->_mousePressPoint = mapToGlobal(mouseEvent->pos());
            if (ElaApplication::containsCursorToItem(this) && mouseEvent->button() == Qt::LeftButton)
            {
                QAction* action = actionAt(mouseEvent->pos());
                if (action && !action->menu())
                {
                    d->_isCloseAnimation = true;
                }
            }
        }
    }
    return QMenu::event(event);
}

void ElaMenu::showEvent(QShowEvent* event)
{
    QPropertyAnimation* posAnimation = new QPropertyAnimation(this, "geometry");
    posAnimation->setEasingCurve(QEasingCurve::InOutSine);
    posAnimation->setDuration(250);
    posAnimation->setStartValue(QRect(QPoint(x() - 15, y()), QSize(1, 1)));
    posAnimation->setEndValue(geometry());
    posAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QMenu::showEvent(event);
}

void ElaMenu::closeEvent(QCloseEvent* event)
{
    Q_D(ElaMenu);
    if (d->_isCloseAnimation)
    {
        d->_isCloseAnimation = false;
        event->ignore();
        QPropertyAnimation* sizeAnimation = new QPropertyAnimation(this, "geometry");
        connect(sizeAnimation, &QPropertyAnimation::finished, this, [=]() {
            close();
        });
        sizeAnimation->setEasingCurve(QEasingCurve::InOutExpo);
        sizeAnimation->setDuration(250);
        sizeAnimation->setStartValue(geometry());
        if (QRect(mapToGlobal(QPoint(0, 0)), QSize(width(), height())).contains(d->_mousePressPoint))
        {
            sizeAnimation->setEndValue(QRect(d->_mousePressPoint, QSize(0, 0)));
        }
        else
        {
            sizeAnimation->setEndValue(QRect(QPoint(x() - 15, y()), QSize(0, 0)));
        }
        sizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        QMenu::closeEvent(event);
    }
}
