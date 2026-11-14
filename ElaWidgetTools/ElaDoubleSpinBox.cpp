#include "ElaDoubleSpinBox.h"

#include "ElaDoubleSpinBoxPrivate.h"
#include "ElaInputFocusAnimation.h"
#include "ElaMenu.h"
#include "ElaSpinBoxStyle.h"
#include "ElaTheme.h"

#include <QContextMenuEvent>
#include <QLineEdit>

ElaDoubleSpinBox::ElaDoubleSpinBox(QWidget* parent)
    : QDoubleSpinBox(parent), d_ptr(new ElaDoubleSpinBoxPrivate())
{
    Q_D(ElaDoubleSpinBox);
    d->q_ptr = this;
    setFixedSize(115, 35);
    d->_style = new ElaSpinBoxStyle(style());
    d->_style->setParent(this);
    setStyle(d->_style);
    new ElaInputFocusAnimation(d->_style, this);
    lineEdit()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lineEdit()->setStyleSheet("background-color:transparent;padding-left:10px;padding-bottom:3px;");
    d->onThemeChanged(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaDoubleSpinBoxPrivate::onThemeChanged);
}

ElaDoubleSpinBox::~ElaDoubleSpinBox() = default;

void ElaDoubleSpinBox::setButtonMode(ElaSpinBoxType::ButtonMode buttonMode)
{
    Q_D(ElaDoubleSpinBox);
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
    d->onThemeChanged(eTheme->getThemeMode());
    setFrame(hasFrame());
    Q_EMIT pButtonModeChanged();
}

ElaSpinBoxType::ButtonMode ElaDoubleSpinBox::getButtonMode() const
{
    Q_D(const ElaDoubleSpinBox);
    return d->_style->getButtonMode();
}

void ElaDoubleSpinBox::paintEvent(QPaintEvent* event)
{
    Q_D(ElaDoubleSpinBox);
    QDoubleSpinBox::paintEvent(event);
}

void ElaDoubleSpinBox::contextMenuEvent(QContextMenuEvent* event)
{
    Q_D(ElaDoubleSpinBox);
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
