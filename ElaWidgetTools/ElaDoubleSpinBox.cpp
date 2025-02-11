#include "ElaDoubleSpinBox.h"

#include "DeveloperComponents/ElaSpinBoxStyle.h"
#include "ElaDoubleSpinBoxPrivate.h"
#include "ElaMenu.h"
#include "ElaTheme.h"
#include <QContextMenuEvent>
#include <QLineEdit>
#include <QTimer>
ElaDoubleSpinBox::ElaDoubleSpinBox(QWidget* parent)
    : QDoubleSpinBox(parent), d_ptr(new ElaDoubleSpinBoxPrivate())
{
    Q_D(ElaDoubleSpinBox);
    d->q_ptr = this;
    setFixedSize(120, 30);
    setStyle(new ElaSpinBoxStyle(style()));
    lineEdit()->setAlignment(Qt::AlignCenter);
    lineEdit()->setStyleSheet("background-color:transparent");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        if (isVisible())
        {
            QPalette palette;
            palette.setColor(QPalette::Base, Qt::transparent);
            palette.setColor(QPalette::Text, ElaThemeColor(themeMode, BasicText));
            lineEdit()->setPalette(palette);
        }
        else
        {
            QTimer::singleShot(1, this, [=] {
                QPalette palette;
                palette.setColor(QPalette::Base, Qt::transparent);
                palette.setColor(QPalette::Text, ElaThemeColor(themeMode, BasicText));
                lineEdit()->setPalette(palette);
            });
        }
    });
}

ElaDoubleSpinBox::~ElaDoubleSpinBox()
{
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
