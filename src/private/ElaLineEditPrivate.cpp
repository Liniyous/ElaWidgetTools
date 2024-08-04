#include "ElaLineEditPrivate.h"

#include <QGraphicsDropShadowEffect>

#include "ElaApplication.h"
#include "ElaLineEdit.h"

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
    if (actionType == ElaAppBarType::WMLBUTTONUP || actionType == ElaAppBarType::WMNCLBUTTONDOWN)
    {
        if (ElaApplication::containsCursorToItem(q) || q->hasSelectedText())
        {
            return;
        }
        q->clearFocus();
        Q_EMIT q->wmFocusOut(q->text());
    }
}

void ElaLineEditPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaLineEdit);
    if (themeMode == ElaThemeType::Light)
    {
        QPalette palette;
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::PlaceholderText, QColor(0x00, 0x00, 0x00, 128));
        q->setPalette(palette);
    }
    else
    {
        QPalette palette;
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::PlaceholderText, QColor(0xBA, 0xBA, 0xBA));
        q->setPalette(palette);
    }
}
