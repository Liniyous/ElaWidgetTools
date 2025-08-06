#include "ElaToolBar.h"

#include <QLayout>
#include <QPainter>
#include <QStyleOption>

#include "ElaIcon.h"
#include "ElaTheme.h"
#include "ElaToolBarPrivate.h"
#include "ElaToolBarStyle.h"
ElaToolBar::ElaToolBar(QWidget* parent)
    : QToolBar(parent), d_ptr(new ElaToolBarPrivate())
{
    Q_D(ElaToolBar);
    d->q_ptr = this;
    setObjectName("ElaToolBar");
    setStyle(new ElaToolBarStyle(style()));
    layout()->setSpacing(10);
    layout()->setContentsMargins(3, 3, 3, 3);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        if (this->isFloating())
        {
            update();
        }
    });
    setAttribute(Qt::WA_TranslucentBackground);

    connect(this, &ElaToolBar::topLevelChanged, this, [=](bool topLevel) {
        if (topLevel)
        {
            layout()->setContentsMargins(d->_shadowBorderWidth + 3, d->_shadowBorderWidth + 3, d->_shadowBorderWidth + 3, d->_shadowBorderWidth + 3);
        }
        else
        {
            layout()->setContentsMargins(3, 3, 3, 3);
        }
    });
}

ElaToolBar::ElaToolBar(const QString& title, QWidget* parent)
    : ElaToolBar(parent)
{
    setWindowTitle(title);
}

ElaToolBar::~ElaToolBar()
{
    delete this->style();
}

void ElaToolBar::setToolBarSpacing(int spacing)
{
    layout()->setSpacing(spacing);
}

int ElaToolBar::getToolBarSpacing() const
{
    return layout()->spacing();
}

QAction* ElaToolBar::addElaIconAction(ElaIconType::IconName icon, const QString& text)
{
    QAction* action = new QAction(text, this);
    action->setProperty("ElaIconType", QChar((unsigned short)icon));
    action->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Broom, 1));
    addAction(action);
    return action;
}

QAction* ElaToolBar::addElaIconAction(ElaIconType::IconName icon, const QString& text, const QKeySequence& shortcut)
{
    QAction* action = new QAction(text, this);
    action->setShortcut(shortcut);
    action->setProperty("ElaIconType", QChar((unsigned short)icon));
    action->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Broom, 1));
    addAction(action);
    return action;
}

void ElaToolBar::paintEvent(QPaintEvent* event)
{
    Q_D(ElaToolBar);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    if (isFloating())
    {
        // 高性能阴影
        eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, 6);
        //背景
        painter.setPen(ElaThemeColor(d->_themeMode, PopupBorder));
        painter.setBrush(ElaThemeColor(d->_themeMode, DialogBase));
        QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * d->_shadowBorderWidth, height() - 2 * d->_shadowBorderWidth);
        painter.drawRoundedRect(foregroundRect, 5, 5);
        QStyle* style = this->style();
        QStyleOptionToolBar opt;
        initStyleOption(&opt);
        opt.rect = style->subElementRect(QStyle::SE_ToolBarHandle, &opt, this);
        if (opt.rect.isValid())
        {
            style->drawPrimitive(QStyle::PE_IndicatorToolBarHandle, &opt, &painter, this);
        }
    }
    else
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::transparent);
        painter.drawRoundedRect(rect(), 5, 5);
        QToolBar::paintEvent(event);
    }
    painter.restore();
}
