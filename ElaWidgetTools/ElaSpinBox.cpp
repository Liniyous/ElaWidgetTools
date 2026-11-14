#include "ElaSpinBox.h"

#include "ElaInputFocusAnimation.h"
#include "ElaMenu.h"
#include "ElaSpinBoxPrivate.h"
#include "ElaSpinBoxStyle.h"
#include "ElaTheme.h"

#include <QContextMenuEvent>
#include <QLineEdit>

ElaSpinBox::ElaSpinBox(QWidget* parent)
    : QSpinBox(parent), d_ptr(new ElaSpinBoxPrivate())
{
    Q_D(ElaSpinBox);
    d->q_ptr = this;
    setFixedSize(115, 35);
    d->_style = new ElaSpinBoxStyle(style());
    d->_style->setParent(this);
    setStyle(d->_style);
    new ElaInputFocusAnimation(d->_style, this);
    lineEdit()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lineEdit()->setStyleSheet("background-color:transparent;padding-left:10px;padding-bottom:3px;");
    d->onThemeChanged(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaSpinBoxPrivate::onThemeChanged);
}

ElaSpinBox::~ElaSpinBox() = default;

void ElaSpinBox::setButtonMode(ElaSpinBoxType::ButtonMode buttonMode)
{
    Q_D(ElaSpinBox);
    if (minimumWidth() < 90)
    {
        setMinimumWidth(90);
    }
    d->_style->setButtonMode(buttonMode);
    switch (buttonMode)
    {
    case ElaSpinBoxType::Inline:
    {
        lineEdit()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        lineEdit()->setStyleSheet("background-color:transparent;padding-left:10px;padding-bottom:3px;");
        break;
    }
    case ElaSpinBoxType::Compact:
    case ElaSpinBoxType::Side:
    case ElaSpinBoxType::PMSide:
    {
        lineEdit()->setAlignment(Qt::AlignCenter);
        lineEdit()->setStyleSheet("background-color:transparent;padding-bottom:3px;");
        break;
    }
    }
    setFrame(hasFrame());
    d->onThemeChanged(eTheme->getThemeMode());
    Q_EMIT pButtonModeChanged();
}

ElaSpinBoxType::ButtonMode ElaSpinBox::getButtonMode() const
{
    Q_D(const ElaSpinBox);
    return d->_style->getButtonMode();
}

void ElaSpinBox::paintEvent(QPaintEvent* event)
{
    Q_D(ElaSpinBox);
    if (palette().color(QPalette::Active, QPalette::Text) != ElaThemeColor(d->_themeMode, BasicText))
    {
        d->onThemeChanged(d->_themeMode);
    }
    QSpinBox::paintEvent(event);
}

void ElaSpinBox::contextMenuEvent(QContextMenuEvent* event)
{
    Q_D(ElaSpinBox);
    ElaMenu* menu = d->_createStandardContextMenu();
    if (!menu)
    {
        return;
    }
    menu->addSeparator();
    const uint se = stepEnabled();
    QAction* up = menu->addElaIconAction(ElaIconType::Plus, tr("增加"));
    up->setEnabled(se & StepUpEnabled);
    QAction* down = menu->addElaIconAction(ElaIconType::Minus, tr("减少"));
    down->setEnabled(se & StepDownEnabled);
    menu->addSeparator();

    const QAbstractSpinBox* that = this;
    const QPoint pos = (event->reason() == QContextMenuEvent::Mouse)
        ? event->globalPos()
        : mapToGlobal(QPoint(event->pos().x(), 0)) + QPoint(width() / 2, height() / 2);
    const QAction* action = menu->exec(pos);
    delete menu;
    if (that && action)
    {
        if (action == up)
        {
            stepBy(1);
        }
        else if (action == down)
        {
            stepBy(-1);
        }
    }
    event->accept();
}
