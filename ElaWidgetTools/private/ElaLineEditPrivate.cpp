#include "ElaLineEditPrivate.h"

#include "ElaApplication.h"
#include "ElaLineEdit.h"
#include "ElaTheme.h"
#include <QTimer>
ElaLineEditPrivate::ElaLineEditPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaLineEditPrivate::~ElaLineEditPrivate()
{
}

void ElaLineEditPrivate::onWMWindowClickedEvent(QVariantMap data)
{
    Q_Q(ElaLineEdit);
    ElaAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<ElaAppBarType::WMMouseActionType>();
    if (actionType == ElaAppBarType::WMLBUTTONDOWN)
    {
        if (q->hasSelectedText() && q->hasFocus())
        {
            q->clearFocus();
        }
    }
    else if (actionType == ElaAppBarType::WMLBUTTONUP || actionType == ElaAppBarType::WMNCLBUTTONDOWN)
    {
        if (ElaApplication::containsCursorToItem(q) || (actionType == ElaAppBarType::WMLBUTTONUP && q->hasSelectedText()))
        {
            return;
        }
        if (q->hasFocus())
        {
            q->clearFocus();
        }
    }
}

void ElaLineEditPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaLineEdit);
    _themeMode = themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::Text, ElaThemeColor(_themeMode, BasicText));
    palette.setColor(QPalette::PlaceholderText, _themeMode == ElaThemeType::Light ? QColor(0x00, 0x00, 0x00, 128) : QColor(0xBA, 0xBA, 0xBA));
    q->setPalette(palette);
}
