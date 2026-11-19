#include "ElaLineEdit.h"

#include "ElaEventBus.h"
#include "ElaInputFocusAnimation.h"
#include "ElaLineEditStyle.h"
#include "ElaMenu.h"
#include "ElaTheme.h"
#include "ElaWidgetToolsDef.h"
#include "private/ElaLineEditPrivate.h"

#include <QClipboard>
#include <QContextMenuEvent>
#include <QDebug>
#include <QGuiApplication>

ELA_PROPERTY_Q_CPP(ElaLineEdit, int, BorderRadius)

ElaLineEdit::ElaLineEdit(QWidget* parent)
    : QLineEdit(parent), d_ptr(new ElaLineEditPrivate())
{
    Q_D(ElaLineEdit);
    d->q_ptr = this;
    setObjectName("ElaLineEdit");
    setFixedHeight(ElaWidgetToolsSize::DefaultControlHeight);
    d->_themeMode = eTheme->getThemeMode();
    d->_pBorderRadius = 6;
    d->_pIsClearButtonEnable = true;
    setFocusPolicy(Qt::StrongFocus);
    // 事件总线
    d->_focusEvent = new ElaEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();
    setMouseTracking(true);
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->_textSpacing);
    setFont(textFont);
    auto* lineEditStyle = new ElaLineEditStyle(style());
    lineEditStyle->setParent(this);
    setStyle(lineEditStyle);
    new ElaInputFocusAnimation(lineEditStyle, this);
    setStyleSheet("#ElaLineEdit{background-color:transparent;padding-left: 10px;}");
    d->onThemeChanged(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaLineEditPrivate::onThemeChanged);
    connect(this, &ElaLineEdit::pBorderRadiusChanged, this, [this]() {
        update();
    });
    QWidget::setVisible(true);
}

ElaLineEdit::~ElaLineEdit() = default;

void ElaLineEdit::setIsClearButtonEnable(bool isClearButtonEnable)
{
    Q_D(ElaLineEdit);
    d->_pIsClearButtonEnable = isClearButtonEnable;
    setClearButtonEnabled(isClearButtonEnable);
    Q_EMIT pIsClearButtonEnableChanged();
}

bool ElaLineEdit::getIsClearButtonEnable() const
{
    Q_D(const ElaLineEdit);
    return d->_pIsClearButtonEnable;
}

void ElaLineEdit::focusInEvent(QFocusEvent* event)
{
    Q_D(ElaLineEdit);
    Q_EMIT focusIn(this->text());
    if (event->reason() != Qt::PopupFocusReason)
    {
        if (d->_pIsClearButtonEnable)
        {
            setClearButtonEnabled(true);
        }
    }
    QLineEdit::focusInEvent(event);
    update();
}

void ElaLineEdit::focusOutEvent(QFocusEvent* event)
{
    Q_D(ElaLineEdit);
    Q_EMIT focusOut(this->text());
    if (event->reason() != Qt::PopupFocusReason)
    {
        if (d->_pIsClearButtonEnable)
        {
            setClearButtonEnabled(false);
        }
        Q_EMIT wmFocusOut(text());
    }
    QLineEdit::focusOutEvent(event);
    update();
}

void ElaLineEdit::paintEvent(QPaintEvent* event)
{
    Q_D(ElaLineEdit);
    if (palette().color(QPalette::Active, QPalette::Text) != ElaThemeColor(d->_themeMode, BasicText))
    {
        d->onThemeChanged(d->_themeMode);
    }
    QLineEdit::paintEvent(event);
}

void ElaLineEdit::contextMenuEvent(QContextMenuEvent* event)
{
    ElaMenu* menu = new ElaMenu(this);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if (!isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::ArrowRotateLeft, tr("撤销"), QKeySequence::Undo);
        action->setEnabled(isUndoAvailable());
        connect(action, &QAction::triggered, this, &ElaLineEdit::undo);

        action = menu->addElaIconAction(ElaIconType::ArrowRotateRight, tr("恢复"), QKeySequence::Redo);
        action->setEnabled(isRedoAvailable());
        connect(action, &QAction::triggered, this, &ElaLineEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if (!isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::KnifeKitchen, tr("剪切"), QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && hasSelectedText() && echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, this, &ElaLineEdit::cut);
    }

    action = menu->addElaIconAction(ElaIconType::Copy, tr("复制"), QKeySequence::Copy);
    action->setEnabled(hasSelectedText() && echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, this, &ElaLineEdit::copy);

    if (!isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::Paste, tr("粘贴"), QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, &ElaLineEdit::paste);
    }
#endif
    if (!isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::DeleteLeft, tr("删除"));
        action->setEnabled(!isReadOnly() && !text().isEmpty() && hasSelectedText());
        connect(action, &QAction::triggered, this, [=](bool checked) {
            if (hasSelectedText())
            {
                int startIndex = selectionStart();
                int endIndex = selectionEnd();
                setText(text().remove(startIndex, endIndex - startIndex));
            }
        });
    }
    if (!menu->isEmpty())
    {
        menu->addSeparator();
    }
    action = menu->addAction(tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!text().isEmpty() && !(selectedText() == text()));
    connect(action, &QAction::triggered, this, &ElaLineEdit::selectAll);
    menu->popup(event->globalPos());
}
