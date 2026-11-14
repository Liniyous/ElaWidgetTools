#include "ElaPlainTextEditPrivate.h"
#include "ElaApplication.h"
#include "ElaPlainTextEdit.h"
#include "ElaTheme.h"
ElaPlainTextEditPrivate::ElaPlainTextEditPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaPlainTextEditPrivate::~ElaPlainTextEditPrivate()
{
}

void ElaPlainTextEditPrivate::onWMWindowClickedEvent(const QVariantMap& data)
{
    Q_Q(ElaPlainTextEdit);
    ElaAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<ElaAppBarType::WMMouseActionType>();
    if (actionType == ElaAppBarType::WMLBUTTONDOWN)
    {
        if (ElaApplication::containsCursorToItem(q))
        {
            return;
        }
        if (!q->textCursor().selectedText().isEmpty() && q->hasFocus())
        {
            q->clearFocus();
        }
    }
    else if (actionType == ElaAppBarType::WMLBUTTONUP || actionType == ElaAppBarType::WMNCLBUTTONDOWN)
    {
        if (ElaApplication::containsCursorToItem(q) || (actionType == ElaAppBarType::WMLBUTTONUP && !q->textCursor().selectedText().isEmpty()))
        {
            return;
        }
        if (q->hasFocus())
        {
            q->clearFocus();
        }
    }
}

void ElaPlainTextEditPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaPlainTextEdit);
    _themeMode = themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::Active, QPalette::Text, ElaThemeColor(_themeMode, BasicText));
    palette.setColor(QPalette::Inactive, QPalette::Text, ElaThemeColor(_themeMode, BasicText));
    palette.setColor(QPalette::Disabled, QPalette::Text, ElaThemeColor(_themeMode, BasicTextDisable));
    palette.setColor(QPalette::PlaceholderText, _themeMode == ElaThemeType::Light ? QColor(0x00, 0x00, 0x00, 128) : QColor(0xBA, 0xBA, 0xBA));
    q->setPalette(palette);
}
