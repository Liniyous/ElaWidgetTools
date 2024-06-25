#include "ElaLineEditPrivate.h"

#include <QGraphicsDropShadowEffect>

#include "ElaApplication.h"
#include "ElaLineEdit.h"
#include "ElaLineEditStyle.h"
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
    Q_UNUSED(data)
    if (ElaApplication::containsCursorToItem(q))
    {
        return;
    }
    q->clearFocus();
}

void ElaLineEditPrivate::onThemeChanged(ElaApplicationType::ThemeMode themeMode)
{
    Q_Q(ElaLineEdit);
    if (themeMode == ElaApplicationType::Light)
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

void ElaLineEditPrivate::_initStyle()
{
    Q_Q(ElaLineEdit);
    q->setClearButtonEnabled(true);
    q->setMouseTracking(true);
    QFont textFont = q->font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, _textSpacing);
    q->setFont(textFont);
    q->setStyle(new ElaLineEditStyle(q->style()));
    q->setStyleSheet(
        "QLineEdit{padding-left: 10px;}");
    onThemeChanged(ElaApplication::getInstance()->getThemeMode());
}
