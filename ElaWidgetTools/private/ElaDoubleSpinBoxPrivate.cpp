#include "ElaDoubleSpinBoxPrivate.h"

#include "ElaDoubleSpinBox.h"
#include "ElaMenu.h"
#include "ElaTheme.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QLineEdit>
#include <QTimer>
ElaDoubleSpinBoxPrivate::ElaDoubleSpinBoxPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaDoubleSpinBoxPrivate::~ElaDoubleSpinBoxPrivate()
{
}

void ElaDoubleSpinBoxPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaDoubleSpinBox);
    _themeMode = themeMode;
    if (q->isVisible())
    {
        _changeTheme();
    }
    else
    {
        QTimer::singleShot(1, this, [=] {
            _changeTheme();
        });
    }
}

ElaMenu* ElaDoubleSpinBoxPrivate::_createStandardContextMenu()
{
    Q_Q(ElaDoubleSpinBox);
    QLineEdit* lineEdit = q->lineEdit();
    ElaMenu* menu = new ElaMenu(q);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if (!lineEdit->isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::ArrowRotateLeft, tr("撤销"), QKeySequence::Undo);
        action->setEnabled(lineEdit->isUndoAvailable());
        connect(action, &QAction::triggered, lineEdit, &QLineEdit::undo);

        action = menu->addElaIconAction(ElaIconType::ArrowRotateRight, tr("恢复"), QKeySequence::Redo);
        action->setEnabled(lineEdit->isRedoAvailable());
        connect(action, &QAction::triggered, lineEdit, &QLineEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if (!lineEdit->isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::KnifeKitchen, tr("剪切"), QKeySequence::Cut);
        action->setEnabled(!lineEdit->isReadOnly() && lineEdit->hasSelectedText() && lineEdit->echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, lineEdit, &QLineEdit::cut);
    }

    action = menu->addElaIconAction(ElaIconType::Copy, tr("复制"), QKeySequence::Copy);
    action->setEnabled(lineEdit->hasSelectedText() && lineEdit->echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, lineEdit, &QLineEdit::copy);

    if (!lineEdit->isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::Paste, tr("粘贴"), QKeySequence::Paste);
        action->setEnabled(!lineEdit->isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, lineEdit, &QLineEdit::paste);
    }
#endif
    if (!lineEdit->isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::DeleteLeft, tr("删除"));
        action->setEnabled(!lineEdit->isReadOnly() && !lineEdit->text().isEmpty() && lineEdit->hasSelectedText());
        connect(action, &QAction::triggered, this, [=](bool checked) {
            if (lineEdit->hasSelectedText())
            {
                int startIndex = lineEdit->selectionStart();
                int endIndex = lineEdit->selectionEnd();
                lineEdit->setText(lineEdit->text().remove(startIndex, endIndex - startIndex));
            }
        });
    }
    if (!menu->isEmpty())
    {
        menu->addSeparator();
    }
    action = menu->addAction(tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!lineEdit->text().isEmpty() && !(lineEdit->selectedText() == lineEdit->text()));
    connect(action, &QAction::triggered, q, &ElaDoubleSpinBox::selectAll);
    return menu;
}

void ElaDoubleSpinBoxPrivate::_changeTheme()
{
    Q_Q(ElaDoubleSpinBox);
    QPalette palette;
    palette.setColor(QPalette::Base, Qt::transparent);
    palette.setColor(QPalette::Text, ElaThemeColor(_themeMode, BasicText));
    q->lineEdit()->setPalette(palette);
}
